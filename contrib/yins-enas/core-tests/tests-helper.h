#ifndef tests_helper
#define tests_helper

// Headers (?)

template <class PGraph>
void CreateSmallGraph(PGraph& Graph) {
  
  // Ensure graph empty
  Graph->Clr();
  
  // Nodes
  Graph->AddNode(0);
  Graph->AddNode(1);
  Graph->AddNode(2);
  Graph->AddNode(3);
  Graph->AddNode(4);
  Graph->AddNode(5);
  Graph->AddNode(6);
  
  // Edges
  Graph->AddEdge(0, 1);
  Graph->AddEdge(1, 2);
  Graph->AddEdge(1, 3);
  Graph->AddEdge(3, 4);
  Graph->AddEdge(3, 5);
  Graph->AddEdge(4, 5);
  Graph->AddEdge(4, 6);
  Graph->AddEdge(5, 6);
  Graph->AddEdge(6, 3);

}

template <class PGraph>
void CreateRandomGraph(PGraph& Graph, const int& Nodes, const int& Edges) {
  
  // Ensure graph empty
  Graph->Clr();
  
  // Variables
  int counter, SrcNId, DstNId;
  
  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  
  // create edges (unique)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    if (SrcNId != DstNId && !Graph->IsEdge(SrcNId, DstNId)) {
      // create edge
      Graph->AddEdge(SrcNId, DstNId);
      counter++;
    }
  }
  
}

template <class PGraph>
void CreateRandomMultiGraph(PGraph& Graph, const int& Nodes, const int& Edges) {
  
  // Ensure graph empty
  Graph->Clr();
  
  // Variables
  int counter, SrcNId, DstNId;
  
  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  
  // create edges (unique)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    if (SrcNId != DstNId) {
      // create edge
      Graph->AddEdge(SrcNId, DstNId);
      counter++;
    }
  }
  
}

template <class TEdgeW, template <class> class TGraph >
void CreateRandomWGraph(TPt<TGraph<TEdgeW> >& Graph, const int& Nodes, const int& Edges, const TEdgeW& MxW, TEdgeW& TotalW) {
  
  // Ensure graph empty
  Graph->Clr();
  TotalW = 0;
  
  // Variables
  int counter, SrcNId, DstNId;
  TEdgeW W;
  
  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }

  // create edges (unique with random weights)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId && !Graph->IsEdge(SrcNId, DstNId)) {
      // create edge
      Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      // weights
      TotalW += W;
    }
  }

}

template <class TEdgeW, template <class> class TGraph >
void CreateRandomWMultiGraph(TPt<TGraph<TEdgeW> >& Graph, const int& Nodes, const int& Edges, const TEdgeW& MxW, TEdgeW& TotalW) {
  
  // Ensure graph empty
  Graph->Clr();
  TotalW = 0;
  
  // Variables
  int counter, SrcNId, DstNId;
  TEdgeW W;
  
  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }

  // create edges (unique with random weights)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId) {
      // create edge
      Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      // weights
      TotalW += W;
    }
  }

}

#endif