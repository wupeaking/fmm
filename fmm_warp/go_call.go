// +build linux
// +build amd64
// +build !noptlogin

package main

/*
#cgo CFLAGS: -I./
#cgo LDFLAGS: -L./ -lfmmwarp
#include <stdlib.h>
#include <stdio.h>
#include "fmm_warp.h" //非标准c头文件，所以用引号
*/
import "C"

import (
	"fmt"
	"unsafe"
)

// CGO_ENABLED=1 go run -v go_call.go
// GOARCH=amd64 GOOS=linux CGO_ENABLED=1 go build -o fmm_go_call go_call.go
// VERBOSE=1
func main() {
	fileNameCStr := C.CString("data/edges.shp")
	idCStr := C.CString("id")
	sourceCStr := C.CString("source")
	targetCStr := C.CString("target")
	ubodtCsvCStr := C.CString("data/ubodt.txt")
	defer C.free(unsafe.Pointer(fileNameCStr))
	defer C.free(unsafe.Pointer(idCStr))
	defer C.free(unsafe.Pointer(sourceCStr))
	defer C.free(unsafe.Pointer(targetCStr))
	defer C.free(unsafe.Pointer(ubodtCsvCStr))

	fmmWarp := C.fmmwarp_new(
		fileNameCStr, idCStr, sourceCStr, targetCStr, ubodtCsvCStr,
		4, 0.4, 0.5, 0,
	)
	defer C.fmmwarp_free(fmmWarp)
	wkt := C.CString("LINESTRING(0.200812146892656 2.14088983050848,1.44262005649717 2.14879943502825,3.06408898305084 2.16066384180791,3.06408898305084 2.7103813559322,3.70872175141242 2.97930790960452,4.11606638418078 2.62337570621469)")
	defer C.free(unsafe.Pointer(wkt))
	C.fmmwarp_match_wkt(fmmWarp, wkt)
}

func bak() {
	// "data/edges.shp", "id", "source", "target"
	fileNameCStr := C.CString("data/edges.shp")
	idCStr := C.CString("id")
	sourceCStr := C.CString("source")
	targetCStr := C.CString("target")
	defer C.free(unsafe.Pointer(fileNameCStr))
	defer C.free(unsafe.Pointer(idCStr))
	defer C.free(unsafe.Pointer(sourceCStr))
	defer C.free(unsafe.Pointer(targetCStr))

	network := C.network_create(fileNameCStr, idCStr, sourceCStr, targetCStr)
	defer C.network_delete(network)
	fmt.Println(C.network_get_node_count(network))

	graph := C.networkgraph_create(network)
	defer C.networkgraph_delete(graph)

	ubdot := C.read_ubodt_csv(C.CString("data/ubodt.txt"))
	// defer C.ubodt_delete(ubdot)
	fmt.Println(C.ubodt_get_delta(ubdot))

	fmm := C.FastMapMatch_create(network, graph, ubdot)
	fmt.Println(fmm)
	//defer C.
	fmt.Println(C.ubodt_get_delta(ubdot))

	fmmCfg := C.FastMapMatchConfig_create(4, 0.4, 0.5, 0)
	fmt.Println(fmmCfg)
	// wkt :=

	C.FastMapMatch_match_wkt(
		fmm,
		C.CString("LINESTRING(0.200812146892656 2.14088983050848,1.44262005649717 2.14879943502825,3.06408898305084 2.16066384180791,3.06408898305084 2.7103813559322,3.70872175141242 2.97930790960452,4.11606638418078 2.62337570621469)"),
		fmmCfg,
	)
}
