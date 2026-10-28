// Package db opens Postgres and applies embedded migrations.
package db

import (
	"context"
	"database/sql"
	"fmt"
	"sort"
	"strings"

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
		if e.IsDir() || !strings.HasSuffix(e.Name(), ".sql") {
			continue
		}
		var n int
		if err := d.QueryRowContext(ctx, `SELECT count(*) FROM schema_migrations WHERE name=$1`, e.Name()).Scan(&n); err != nil {
			return fmt.Errorf("check migration %s: %w", e.Name(), err)
		}
		if n > 0 {
			continue
		}
		b, err := migrations.FS.ReadFile(e.Name())
		if err != nil {
			return err
		}
		// Apply each migration and its bookkeeping row atomically.
		tx, err := d.BeginTx(ctx, nil)
		if err != nil {
			return err
		}
		if _, err := tx.ExecContext(ctx, string(b)); err != nil {
			_ = tx.Rollback()
			return fmt.Errorf("apply migration %s: %w", e.Name(), err)
		}
		if _, err := tx.ExecContext(ctx, `INSERT INTO schema_migrations(name) VALUES ($1)`, e.Name()); err != nil {
			_ = tx.Rollback()
			return err
		}
		if err := tx.Commit(); err != nil {
			return err
		}
	}
	return nil
}
