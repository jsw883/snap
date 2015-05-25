Amazon product co-purchasing network, March 02 2003
---------------------------------------------------

### Source ###

[SNAP Datasets » amazon0302](https://snap.stanford.edu/data/amazon0302.html)

### Code ###

```bash
wget --user-agent="Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36" https://snap.stanford.edu/data/amazon0302.txt.gz
gunzip amazon0302.txt.gz
mv amazon0302.txt amazon0302.snap
```

### Description ###

Network was collected by crawling Amazon website. It is based on Customers Who Bought This Item Also Bought feature of the Amazon website. If a product i is frequently co-purchased with product j, the graph contains a directed edge from i to j.

The data was collected in March 02 2003.

|                                  |                 |
| -------------------------------- | --------------- |
| Nodes                            | 262111          |
| Edges                            | 1234877         |
| Nodes in largest WCC             | 262111 (1.000)  |
| Edges in largest WCC             | 1234877 (1.000) |
| Nodes in largest SCC             | 241761 (0.922)  |
| Edges in largest SCC             | 1131217 (0.916) |
| Average clustering coefficient   | 0.4198          |
| Number of triangles              | 717719          |
| Fraction of closed triangles     | 0.09339         |
| Diameter (longest shortest path) | 32              |
| 90-percentile effective diameter | 11              |
