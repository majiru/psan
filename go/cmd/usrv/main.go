package main

import (
	"bufio"
	"encoding/json"
	"io"
	"log"
	"net"
	"os"
	"strings"

	psan "github.com/majiru/psan/go"
)

const sock = "/tmp/db-serv.sock"

func handle(br *bufio.Reader, out io.Writer) {
	for {
		s, err := br.ReadString('\n')
		switch err {
		case io.EOF:
			return
		case nil:
		default:
			log.Println("non eof error when reading input:", err)
			return
		}
		psan.DBact(s)
		switch strings.HasPrefix(s, "Get") {
		case true:
			o := psan.DBget()
			if o == nil {
				log.Println("request for empty object")
				continue
			}
			b, err := json.Marshal(o)
			if err != nil {
				log.Println("error when marshalling json:", err)
				continue
			}
			out.Write(b)
		case false:
			out.Write([]byte("ACK"))
		}
	}
}

func main() {
	if err := os.RemoveAll(sock); err != nil {
		log.Fatal(err)
	}
	l, err := net.Listen("unix", sock)
	if err != nil {
		log.Fatal(err)
	}
	defer l.Close()
	for {
		conn, err := l.Accept()
		if err != nil {
			log.Fatal(err)
		}
		handle(bufio.NewReader(conn), conn)
	}
}
