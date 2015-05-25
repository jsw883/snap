Amazon product co-purchasing network, March 12 2003
---------------------------------------------------

### Source ###

[SNAP Datasets » amazon0302](https://snap.stanford.edu/data/amazon0302.html)

### Code ###

```bash
wget --user-agent="Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36" https://snap.stanford.edu/data/amazon0312.txt.gz
gunzip amazon0312.txt.gz
mv amazon0312.txt amazon0312.snap
```

### Description ###

Network was collected by crawling Amazon website. It is based on Customers Who Bought This Item Also Bought feature of the Amazon website. If a product i is frequently co-purchased with product j, the graph contains a directed edge from i to j.

The data was collected in March 12 2003.

|                                  |                 |
| -------------------------------- | --------------- |
| Nodes                            | 400727          |
| Edges                            | 3200440         |
| Nodes in largest WCC             | 400727 (1.000)  |
| Edges in largest WCC             | 3200440 (1.000) |
| Nodes in largest SCC             | 380167 (0.949)  |
| Edges in largest SCC             | 3069889 (0.959) |
| Average clustering coefficient   | 0.4022          |
| Number of triangles              | 3686467         |
| Fraction of closed triangles     | 0.05991         |
| Diameter (longest shortest path) | 18              |
| 90-percentile effective diameter | 7.6             |
