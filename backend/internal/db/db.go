// Package db opens Postgres and applies embedded migrations.
package db

import (
	"context"
	"database/sql"
	"sort"

	"github.com/doubleastandard9093-debug/eve-elite-ogame-design/backend/migrations"
	_ "github.com/jackc/pgx/v5/stdlib"
)

func Open(url string) (*sql.DB, error) { return sql.Open("pgx", url) }

func Migrate(ctx context.Context, d *sql.DB) error {
	if _, err := d.ExecContext(ctx, `CREATE TABLE IF NOT EXISTS schema_migrations (name TEXT PRIMARY KEY, applied_at TIMESTAMPTZ DEFAULT now())`); err != nil {
		return err
	}
	entries, err := migrations.FS.ReadDir(".")
	if err != nil {
		return err
	}
	sort.Slice(entries, func(i, j int) bool { return entries[i].Name() < entries[j].Name() })
	for _, e := range entries {
		var n int
		_ = d.QueryRowContext(ctx, `SELECT count(*) FROM schema_migrations WHERE name=$1`, e.Name()).Scan(&n)
		if n > 0 {
			continue
		}
		b, err := migrations.FS.ReadFile(e.Name())
		if err != nil {
			return err
		}
		if _, err := d.ExecContext(ctx, string(b)); err != nil {
			return err
		}
		if _, err := d.ExecContext(ctx, `INSERT INTO schema_migrations(name) VALUES ($1)`, e.Name()); err != nil {
			return err
		}
	}
	return nil
}
