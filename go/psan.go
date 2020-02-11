package psan

// #include <psan.h>
// #include <stdlib.h>
// #cgo LDFLAGS: -l psan
import "C"
import (
	"unsafe"
)

const (
	STR = iota
	NUM
	OBJ
)

type Pair struct {
	Key  string  `json:"key"`
	Type uint    `json:"type"`
	Sval string  `json:"sval,omitempty"`
	Ival int64   `json:"ival,omitempty"`
	Oval *Object `json:"oval,omitempty"`
}

type Object struct {
	Name  string `json:"name"`
	Npair int    `json:"npair"`
	Pairs []Pair `json:"pairs"`
}

func DBact(s string) {
	cs := C.CString(s)
	defer C.free(unsafe.Pointer(cs))
	C.dbput(cs)
}

func Fdbact(fd uintptr) {
	C.fdbput(C.int(fd))
}

func parsePair(p unsafe.Pointer) *Pair {
	pair := &Pair{}
	var cpair *C.Pair = (*C.Pair)(p)
	pair.Key = C.GoString(cpair.key)
	pair.Type = uint(cpair._type)
	switch pair.Type {
	case STR:
		pair.Sval = C.GoString(*(**C.char)(unsafe.Pointer(&cpair.anon0)))
	case NUM:
		pair.Ival = int64(*(*C.int)(unsafe.Pointer(&cpair.anon0)))
	}
	return pair
}

func parseObj(ptr unsafe.Pointer) *Object {
	o := &Object{}
	var co *C.Object = (*C.Object)(ptr)
	o.Name = C.GoString(co.name)
	o.Npair = int(co.npair)
	var p unsafe.Pointer = unsafe.Pointer(co.pairs)
	for i := 0; i < o.Npair; i++ {
		o.Pairs = append(o.Pairs, *parsePair(p))
		p = unsafe.Pointer(uintptr(p) + C.sizeof_Pair)
	}
	return o
}

func DBget() *Object {
	co := C.dbget(nil)
	if co == nil {
		return nil
	}
	return parseObj(unsafe.Pointer(co))
}
