Amazon product co-purchasing network, March 02 2003
---------------------------------------------------

### Source ###

[SNAP Datasets » amazon0302](https://snap.stanford.edu/data/amazon0302.html)

### Code ###

```bash
wget --user-agent="Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36" https://snap.stanford.edu/data/amazon0302.txt.gz
gunzip amazon0302.txt.gz
awk '{ sub("\r$", ""); print }' amazon0302.txt > amazon0302.snap
rm amazon0302.txt
awk 'NR < 4 {print $0} NR == 4 {print $0"\tEdgeW"} NR > 4 {print $0"\t1"}' amazon0302.snap > amazon0302.snap.IntW
awk 'NR < 4 {print $0} NR == 4 {print $0"\tEdgeW"} NR > 4 {print $0"\t1.0"}' amazon0302.snap > amazon0302.snap.FltW
```

### Description ###

Network was collected by crawling Amazon website. It is based on Customers Who Bought This Item Also Bought feature of the Amazon website. If a product i is frequently co-purchased with product j, the graph contains a directed edge from i to j.

The data was collected in March 02 2003.
