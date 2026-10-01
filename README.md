# Final Fantasy Tactics decompilation

> Status: 100% decompiled and byte-exact ✅

## About this fork

A fork of [adamrt/fft_decomp](https://github.com/adamrt/fft_decomp) focused on
the semantic cleanup that upstream lists as its next step: naming what is still
unnamed, documenting what each value is for, giving data its real types, and
turning leftover `goto`s and register pins into ordinary C. The goal is source
you can read, modify and port without reverse-engineering it again.

The rule is the same as upstream: every commit still builds the original disc
byte for byte. Work lands on `semantic-cleanup` in small commits, each checked
with `make validate` before it goes in; `master` mirrors upstream.

A matching decompilation of the North American PlayStation release of *Final
Fantasy Tactics* (`SCUS-94221` [redump](http://redump.org/disc/55/)). Every game
function is C that compiles to the original bytes, and the rebuilt disc is a
byte-for-byte match.

Game files and proprietary Sony tools are not included.

## Next Steps

The game is 100% completely decompiled, but there are plenty of semantic changes
and cleanup to be done. Variable and function names, correcting types, improving
enum usage/naming, etc.

Each time a change is made `make validate` will ensure the changes still match
byte-for-byte.

## Getting started

Requirements
- Docker
- `SCUS-94221` BIN at `./scus-94221.bin` (optional)

Then run:

```sh
# Prepare environment
make bootstrap

# Then use one of the following (make help for more):
make validate  # verify all code against checksums (without BIN)
make build     # build every module and a byte-matching disc (needs the BIN)
```

`make build` writes `build/disc/output-scus-94221.bin` and `.cue`.

## Thanks

This project is an ode to the [FFHacktics](https://ffhacktics.com/wiki/)
community. This project would not be possible without it. ❤️

And thanks to Adam ([@adamrt](https://github.com/adamrt)) for the decompilation
this fork builds on, and for the go-ahead to fork it. 🙏⚔️✨
