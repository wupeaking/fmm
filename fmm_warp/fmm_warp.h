#ifdef __cplusplus
extern "C"
{
#endif

    void *network_create(char *filename, char *id, char *source, char *target);
    void network_delete(void *network);
    int network_get_node_count(void *network);

    void *networkgraph_create(void *network);
    void networkgraph_delete(void *graph);

    void *read_ubodt_csv(char *filename);
    void ubodt_delete(void *ubodt);
    double ubodt_get_delta(void *ubodt);

    void *FastMapMatch_create(void *network, void *graph, void *ubodt);
    void *FastMapMatch_create_shared_ubodt(void *network, void *graph, void *ubodt_shared_ptr);
    void FastMapMatch_match_wkt(void *fmm, char *wkt, void *fmm_cfg);

    void *FastMapMatchConfig_create(int k_arg, double r_arg, double gps_error, double reverse_tolerance);
    void FastMapMatchConfig_delete(void *fmm_cfg);

    // 使用下面这两个
    void *fmmwarp_new(char *filename, char *id,
                      char *source, char *target, char *ubodt_csv,
                      int k_arg, double r_arg, double gps_error, double reverse_tolerance);
    void fmmwarp_free(void *fmm_warp);
    void fmmwarp_match_wkt(void *fmm_warp, char *wkt);
#ifdef __cplusplus
}
#endif