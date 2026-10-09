package main

import (
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestCheckWorkflowShardsRequiresEveryModuleOnce(t *testing.T) {
	workflow := func(shards ...string) string {
		text := "jobs:\n  validate:\n    strategy:\n      matrix:\n        shard:\n"
		for _, modules := range shards {
			text += "          - { name: shard, modules: " + modules + " }\n"
		}
		return text
	}
	tests := []struct {
		name   string
		shards []string
		want   string
	}{
		{name: "complete", shards: []string{"battle", "main event"}},
		{name: "missing", shards: []string{"battle", "main"}, want: "2 modules are in no validate shard: event-attack event-card"},
		{name: "doubled", shards: []string{"battle main", "main event"}, want: "module main is validated by two shards"},
		{name: "unknown", shards: []string{"battle main event", "nowhere"}, want: "unknown module"},
		{name: "no shards", want: "no validate shard"},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			p := validTestProject(t)
			path := filepath.Join(p.root, filepath.FromSlash(workflowFile))
			if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
				t.Fatal(err)
			}
			if err := os.WriteFile(path, []byte(workflow(test.shards...)), 0o644); err != nil {
				t.Fatal(err)
			}
			config, err := p.loadProjectConfig()
			if err != nil {
				t.Fatal(err)
			}
			count, err := p.checkWorkflowShards(config)
			if test.want == "" {
				if err != nil || count != len(test.shards) {
					t.Fatalf("count = %d, err = %v", count, err)
				}
				return
			}
			if err == nil || !strings.Contains(err.Error(), test.want) {
				t.Fatalf("err = %v, want %q", err, test.want)
			}
		})
	}
}

func TestCheckWorkflowShardsSkipsACheckoutWithoutWorkflow(t *testing.T) {
	p := validTestProject(t)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	if count, err := p.checkWorkflowShards(config); count != 0 || err != nil {
		t.Fatalf("count = %d, err = %v", count, err)
	}
}
