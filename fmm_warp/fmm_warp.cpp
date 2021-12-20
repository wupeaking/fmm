#include <fmm/fmm-api.hpp>
#include <iostream>
#include "fmm_warp.h"
#include <fmm/mm/fmm/ubodt.hpp>
#include <fmm/mm/fmm/fmm_algorithm.hpp>
#include <fmm/python/pyfmm.hpp>

#ifdef __cplusplus
extern "C"{
#endif

using namespace FMM;
using namespace FMM::NETWORK;
using namespace FMM::CORE;
using namespace FMM::MM;


class FMMWarp {
    public:
       NETWORK::Network* network;
       NETWORK::NetworkGraph* graph;
       std::shared_ptr<UBODT> ubodt;
       FastMapMatch* fmm;
       FastMapMatchConfig* fmm_cfg;
};

void* fmmwarp_new(char* filename, char* id,
 char* source, char* target, char* ubodt_csv,
 int k_arg, double r_arg, double gps_error, double reverse_tolerance) {
    FMMWarp* fmm_warp = new FMMWarp();
    std::string file_str(filename);
    std::string id_str(id);
    std::string source_str(source);
    std::string target_str(target);
    fmm_warp->network = new NETWORK::Network(file_str, id_str, source_str, target_str);
    fmm_warp->graph = new NETWORK::NetworkGraph(*(fmm_warp->network));

    std::string ubodt_file(ubodt_csv);
    fmm_warp->ubodt = UBODT::read_ubodt_csv(ubodt_file);
    fmm_warp->fmm_cfg =  new FastMapMatchConfig(k_arg, r_arg, gps_error, reverse_tolerance);
    fmm_warp->fmm = new FastMapMatch(
        *(fmm_warp->network),
        *(fmm_warp->graph),
        fmm_warp->ubodt
    );
    return fmm_warp;
}

void fmmwarp_free(void *fmm_warp){
    FMMWarp* warp = static_cast<FMMWarp* >(fmm_warp);
    delete warp->network;
    delete warp->graph;
    delete warp->fmm;
    delete warp->fmm_cfg;
    delete warp;
}

void fmmwarp_match_wkt(void* fmm_warp, char* wkt) {
    std::string wkt_str(wkt);
    FMMWarp* warp = static_cast<FMMWarp* >(fmm_warp);

    PYTHON::PyMatchResult result = 
    warp->fmm->match_wkt(
        wkt_str,
        *(warp->fmm_cfg)
    );
    SPDLOG_INFO("Matched path: {} ", result.cpath);
}

 

void *network_create(char* filename, char* id, char* source, char* target) {
    std::string file_str(filename);
    std::string id_str(id);
    std::string source_str(source);
    std::string target_str(target);
    return new NETWORK::Network(file_str, id_str, source_str, target_str); //构造
}

void network_delete(void* network){
    delete static_cast<NETWORK::Network *>(network);
}

int network_get_node_count(void* network) {
    return static_cast<NETWORK::Network *>(network)->get_node_count();
}

void* networkgraph_create(void* network) {
    // FMM::NETWORK::Network& p = static_cast<NETWORK::Network *>(network);
    return new NETWORK::NetworkGraph(*(static_cast<NETWORK::Network *>(network)));
}

void networkgraph_delete(void* graph) {
    delete static_cast<NETWORK::NetworkGraph *>(graph);
}

void* read_ubodt_csv(char* filename){
    std::string file_str(filename);
    std::shared_ptr<UBODT> u =  UBODT::read_ubodt_csv(file_str);
    UBODT *ptr = &*u;
    // UBODT *ptr = u.get();
    u.reset();
    // u = nullptr;
    return ptr;
}


void ubodt_delete(void* ubodt) {
    delete static_cast<UBODT *>(ubodt);
}

double ubodt_get_delta(void* ubodt){
    return static_cast<UBODT *>(ubodt)->get_delta();
}


void* FastMapMatch_create(void* network, void* graph, void* ubodt){
    std::shared_ptr<UBODT> ubodt_shared_ptr((UBODT* )ubodt);
    // ubodt_shared_ptr.reset((UBODT *)ubodt);
    std::cout << "FastMapMatch_create----" << std::endl;
    std::cout << ubodt_shared_ptr->get_delta() << std::endl;

    return new FastMapMatch(
        *(static_cast<NETWORK::Network *>(network)),
        *(static_cast<NETWORK::NetworkGraph *>(graph)),
        ubodt_shared_ptr
    );
}

void* FastMapMatch_create_shared_ubodt(void* network, void* graph, void* ubodt_shared_ptr){
        return new FastMapMatch(
        *(static_cast<NETWORK::Network *>(network)),
        *(static_cast<NETWORK::NetworkGraph *>(graph)),
        *(std::shared_ptr<UBODT>*)ubodt_shared_ptr
    );
}


void FastMapMatch_match_wkt(void* fmm, char* wkt, void* fmm_cfg) {
    std::cout<< "1 -----------------" << std::endl;
    std::string file_str(wkt);
    std::cout<< "2 -----------------" << std::endl;
    // PYTHON::PyMatchResult result = 
    static_cast<FastMapMatch *>(fmm)->match_wkt(
        file_str,
        *(static_cast<FastMapMatchConfig *>(fmm_cfg))
    );
    std::cout<< "3 -----------------" << std::endl;
    // SPDLOG_INFO("Matched path: {} ", result.cpath);
}

void* FastMapMatchConfig_create(int k_arg, double r_arg, double gps_error, double reverse_tolerance) {
    return new FastMapMatchConfig(k_arg, r_arg, gps_error, reverse_tolerance);
}

void FastMapMatchConfig_delete(void* fmm_cfg) {
    delete static_cast<FastMapMatchConfig *>(fmm_cfg);
}
#ifdef __cplusplus
}
#endif

/*
g++ -I/usr/include/gdal -std=gnu++11 -fopenmp main.cpp  -o fmm_print_network  \
-Wl,-rpath,/usr/local/lib: /usr/local/lib/libFMMLIB.so /usr/lib/libgdal.so \
/usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0 \
/usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so \
/usr/lib/x86_64-linux-gnu/libpthread.so


生成动态链接库
g++ -I/usr/include/gdal -I. -std=gnu++11 -fopenmp -fPIC -shared fmm_warp.cpp -o libfmmwarp.so \
-Wl,-rpath,/usr/local/lib: /usr/local/lib/libFMMLIB.so /usr/lib/libgdal.so \
/usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0 \
/usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so \
/usr/lib/x86_64-linux-gnu/libpthread.so
*/