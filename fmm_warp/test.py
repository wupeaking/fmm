#coding=utf-8
from fmm import Network,NetworkGraph,FastMapMatch,FastMapMatchConfig,UBODT
network = Network("data/edges.shp")
print "Nodes {} edges {}".format(network.get_node_count(),network.get_edge_count())
graph = NetworkGraph(network)
ubodt = UBODT.read_ubodt_csv("data/ubodt.txt")
model = FastMapMatch(network,graph,ubodt)
k = 4
radius = 0.4
gps_error = 0.5
fmm_config = FastMapMatchConfig(k,radius,gps_error)
wkt = "LINESTRING(0.200812146892656 2.14088983050848,1.44262005649717 2.14879943502825,3.06408898305084 2.16066384180791,3.06408898305084 2.7103813559322,3.70872175141242 2.97930790960452,4.11606638418078 2.62337570621469)"
result = model.match_wkt(wkt,fmm_config)

print "Matched path: ", list(result.cpath)
print "Matched edge for each point: ", list(result.opath)
print "Matched edge index ",list(result.indices)
print "Matched geometry: ",result.mgeom.export_wkt()
print "Matched point ", result.pgeom.export_wkt()
print(result)