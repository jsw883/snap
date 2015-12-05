#ifndef tests_helper
#define tests_helper

// Headers (?)

template <class PGraph>
void CreateSmallGraph(PGraph& Graph) {
  
  // Ensure graph empty
  
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

#endif