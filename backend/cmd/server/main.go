package main

import (
	"context"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/doubleastandard9093-debug/eve-elite-ogame-design/backend/internal/api"
	"github.com/doubleastandard9093-debug/eve-elite-ogame-design/backend/internal/db"
)

func main() {
	url := getenv("DATABASE_URL", "postgres://helios:helios@localhost:5432/helios?sslmode=disable")
	d, err := db.Open(url)
	if err != nil {
		log.Fatal(err)
	}
	ctx, cancel := context.WithTimeout(context.Background(), 30*time.Second)
	defer cancel()
	for i := 0; ; i++ {
		if err = db.Migrate(ctx, d); err == nil || i > 10 {
			break
		}
		log.Printf("waiting for db: %v", err)
		time.Sleep(2 * time.Second)
	}
	if err != nil {
		log.Fatalf("migrate: %v", err)
	}
	addr := ":" + getenv("PORT", "8080")
	log.Printf("helios backend listening on %s", addr)
	log.Fatal(http.ListenAndServe(addr, api.NewRouter(d, "0.1.0")))
}

func getenv(k, def string) string {
	if v := os.Getenv(k); v != "" {
		return v
	}
	return def
}
