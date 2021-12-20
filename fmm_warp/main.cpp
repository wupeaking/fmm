#include <fmm/fmm-api.hpp>
#include <iostream>
#include <fmm/mm/fmm/ubodt.hpp>
#include <fmm/mm/fmm/fmm_algorithm.hpp>
#include <fmm/python/pyfmm.hpp>

using namespace FMM;
using namespace FMM::NETWORK;
using namespace FMM::CORE;
using namespace FMM::MM;

int main(int argc, char **argv){
  SPDLOG_INFO("Use FMM in an external project");

    // std::string filename(argv[1]);
    // SPDLOG_INFO("Network file {}",filename);
    Network network("data/edges.shp","id","source","target");
    NETWORK::NetworkGraph graph(network);
    std::shared_ptr<UBODT> u =  UBODT::read_ubodt_csv("data/ubodt.txt");
    FastMapMatch fmm(
        network,
        graph,
        u
    );

   FastMapMatchConfig cfg(4, 0.4, 0.5, 0);
   PYTHON::PyMatchResult result = fmm.match_wkt(
     "LINESTRING(0.200812146892656 2.14088983050848,1.44262005649717 2.14879943502825,3.06408898305084 2.16066384180791,3.06408898305084 2.7103813559322,3.70872175141242 2.97930790960452,4.11606638418078 2.62337570621469)",
     cfg
   );
   SPDLOG_INFO("Matched path: {} ", result.cpath);
};