US Airport Network 2010
-----------------------

### Source ###

[Tore Opsahl » Datasets](http://toreopsahl.com/datasets/#usairports)

### Code ###

```bash
wget --user-agent="Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36" http://opsahl.co.uk/tnet/datasets/USairport_2010.txt
(echo $'# Weighted directed graph: USairport2010.snap\n# Dataset containing the complete US airport network in 2010, where weights represent number of seats available on scheduled flights between airports (weights of duplicate edges are summed).\n# Nodes: 1574 Edges: 28236\n# FromNodeId  ToNodeId  EdgeW'; cat USairport_2010.txt) > USairport2010.snap
rm USairport_2010.txt
```

### Description ###

This dataset is the complete US airport network in 2010. This is the network used in the first part of the [Why Anchorage is not (that) important: Binary ties and Sample selection](http://toreopsahl.com/2011/08/12/why-anchorage-is-not-that-important-binary-ties-and-sample-selection/)-blog post. The data is downloaded from the Bureau of Transportation Statistics (BTS) Transtats site ([Table T-100; id 292](http://www.transtats.bts.gov/DL_SelectFields.asp?Table_ID=292)) with the following filters: Geography=all; Year=2010; Months=all; and columns: Passengers, Origin, Dest. Based on this table, the [airport codes](http://opsahl.co.uk/tnet/datasets/USairport_2010_codes.txt) are converted into id numbers, and the weights of duplicated ties are summed up. Also ties with a weight of 0 are removed (only cargo), and self-loops removed.
