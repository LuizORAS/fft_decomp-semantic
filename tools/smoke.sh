#!/bin/sh
# The host side of `make smoke`: boot each run that `tools smoke prepare`
# listed in build/smoke/runs.txt in PCSX-Redux ($PCSX_REDUX) with
# tools/smoke.lua, and close an emulator whose log.txt stops for
# $SMOKE_WATCHDOG seconds. world-map continues from the save new-game wrote
# on the same disc. Under WSL the emulator is a Windows program: it gets
# Windows paths, and the watchdog closes the pcsx-redux.main process that its
# launcher started.
set -u
: "${PCSX_REDUX:?set PCSX_REDUX to the PCSX-Redux executable}"
watchdog=${SMOKE_WATCHDOG:-150}

if command -v wslpath >/dev/null 2>&1; then
	host_path() { wslpath -w "$1"; }
	separator='\'
else
	host_path() { printf '%s\n' "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"; }
	separator=/
fi

# The emulator processes running now (WSL: Windows pcsx-redux.main ids).
emulators() {
	if command -v tasklist.exe >/dev/null 2>&1; then
		tasklist.exe /FI "IMAGENAME eq pcsx-redux.main" /FO CSV /NH </dev/null 2>/dev/null |
			tr -d '\r"' | cut -d, -f2 | grep -E '^[0-9]+$'
	fi
}

close_new_emulators() {
	if command -v tasklist.exe >/dev/null 2>&1; then
		for id in $(emulators); do
			case "$1" in *" $id "*) ;; *) taskkill.exe /F /PID "$id" </dev/null >/dev/null 2>&1 ;; esac
		done
	else
		kill "$2" 2>/dev/null
	fi
}

lua=$(host_path tools/smoke.lua)
while read -r disc session cue; do
	dir=build/smoke/$disc/$session
	if [ "$session" = world-map ]; then
		card=build/smoke/$disc/new-game/memcard1.mcd
		if ! grep -q BASCUS-94221 "$card" 2>/dev/null; then
			echo "smoke: $disc $session skipped: new-game wrote no save"
			continue
		fi
		cp "$card" "$dir/memcard1.mcd" || exit 1
	fi
	win=$(host_path "$dir")
	iso=$(host_path "$cue")
	before=" $(emulators | tr '\n' ' ') "
	echo "smoke: $disc $session started at $(date +%T)"
	# From the session's directory: the emulator writes its crash reporter's
	# files into its working directory.
	(cd "$dir" && exec "$PCSX_REDUX" -portable "$win" -iso "$iso" \
		-memcard1 "$win${separator}memcard1.mcd" -memcard2 "$win${separator}memcard2.mcd" \
		-interpreter -debugger -run -logfile "$win${separator}emulator.log" \
		-dofile "$lua" -exec "smoke_run([[$win]], '$session')" </dev/null) &
	pid=$!
	# The script's heartbeat adds a line to log.txt every 1800 frames.
	size=none
	changed=$(date +%s)
	while kill -0 "$pid" 2>/dev/null; do
		sleep 10
		now=none
		[ -f "$dir/log.txt" ] && now=$(wc -c <"$dir/log.txt")
		if [ "$now" != "$size" ]; then
			size=$now
			changed=$(date +%s)
		elif [ $(($(date +%s) - changed)) -gt "$watchdog" ]; then
			echo "smoke: $disc $session: log.txt unchanged for $watchdog s; closing the emulator"
			close_new_emulators "$before" "$pid"
		fi
	done
	wait "$pid"
	echo "smoke: $disc $session ended at $(date +%T) (exit $?)"
done <build/smoke/runs.txt
