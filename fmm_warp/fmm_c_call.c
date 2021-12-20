#include "fmm_warp.h"
#include <stdio.h>

extern void test_1();
int main() {
    // void* network = network_create("data/edges.shp", "id", "source", "target");
    // printf("--------- %d\n", network_get_node_count(network));
    // void* graph = networkgraph_create(network);
    // void* ubdot = read_ubodt_csv("data/ubodt.txt");
    // printf("--ubdot------- %p\n", ubdot);
    // ubodt_delete(ubdot);
  //   void* fmm = FastMapMatch_create(network, graph, ubdot);
  //   void* fmm_cfg = FastMapMatchConfig_create(4, 0.4, 0.5, 0);
  //   FastMapMatch_match_wkt(
	// 	fmm,
	// 	"LINESTRING(0.200812146892656 2.14088983050848,1.44262005649717 2.14879943502825,3.06408898305084 2.16066384180791,3.06408898305084 2.7103813559322,3.70872175141242 2.97930790960452,4.11606638418078 2.62337570621469)",
	// 	fmm_cfg
	// );
test_1();

}

void test_1() {
  void* fmmwarp =  fmmwarp_new("data/edges.shp", "id", "source", "target",
   "data/ubodt.txt", 4, 0.4, 0.5, 0);
  fmmwarp_match_wkt(fmmwarp,
  "LINESTRING(0.200812146892656 2.14088983050848,1.44262005649717 2.14879943502825,3.06408898305084 2.16066384180791,3.06408898305084 2.7103813559322,3.70872175141242 2.97930790960452,4.11606638418078 2.62337570621469)"
  );
}

/*
gcc -I/usr/include/gdal -I. -fopenmp fmm_c_call.c  -o fmm_c  \
-Wl,-rpath,/usr/local/lib:./: /usr/local/lib/libFMMLIB.so /usr/lib/libgdal.so \
/usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0 \
/usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so \
/usr/lib/x86_64-linux-gnu/libpthread.so ./libfmmwarp.so


gcc -I/usr/include/gdal -I. fmm_c_call.c -L. -lfmmwarp  -o fmm_c 


执行之前
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/wupengxin/fmm_warp
*/