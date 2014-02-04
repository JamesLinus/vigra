/************************************************************************/
/*                                                                      */
/*                 Copyright 2011 by Ullrich Koethe                     */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://hci.iwr.uni-heidelberg.de/vigra/                       */
/*    Please direct questions, bug reports, and contributions to        */
/*        ullrich.koethe@iwr.uni-heidelberg.de    or                    */
/*        vigra@informatik.uni-hamburg.de                               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

/**
 * This header provides definitions of graph-related algorithms
 */

#ifndef VIGRA_GRAPH_MAP_ALGORITHMS_HXX
#define VIGRA_GRAPH_MAP_ALGORITHMS_HXX


#include <vigra/graphs.hxx>
#include <vigra/graph_generalization.hxx>
#include <vigra/multi_gridgraph.hxx>
#include <vigra/numpy_array.hxx>
#include <vigra/priority_queue.hxx>

#include <boost/iterator/transform_iterator.hpp>

namespace vigra{

    template<class GRAPH,class MAP>
    class EdgeMapIteratorHelper{
    public:
        typedef typename GraphMapTypeTraits<MAP>::Reference      Reference;
        typedef typename GraphMapTypeTraits<MAP>::ConstReference ConstReference;
        typedef typename GraphMapTypeTraits<MAP>::Value          Value;
    private:
        struct Transform{
            
            Transform(MAP & map)
            : map_(&map){
            }
            template<class ITEM>
            Reference operator()(const ITEM & item)const{
                return  map_->operator[](item);
            }
            mutable MAP * map_;
        };  
        struct ConstTransform{
            
            ConstTransform(const MAP & map)
            : map_(&map){
            }
            template<class ITEM>
            ConstReference operator()(const ITEM & item)const{
                return  map_->operator[](item);
            }
            const MAP * map_;
        }; 
    public:
        typedef  boost::transform_iterator< Transform,      typename GRAPH::EdgeIt,Reference      ,Value> iterator;
        typedef  boost::transform_iterator< ConstTransform, typename GRAPH::EdgeIt,ConstReference ,Value> const_iterator;
        static iterator
        begin(const GRAPH & g, MAP & map){
            Transform f(map);
            typename  GRAPH::EdgeIt iter = GraphIteratorAccessor<GRAPH>::edgesBegin(g);
            return iterator(iter,f);
        }
        static iterator
        end(const GRAPH & g, MAP & map){
            Transform f(map);
            typename  GRAPH::EdgeIt iter = GraphIteratorAccessor<GRAPH>::edgesEnd(g);
            return iterator(iter,f);
        }
        static const_iterator
        begin(const GRAPH & g, const MAP & map){
            ConstTransform f(map);
            typename  GRAPH::EdgeIt iter = GraphIteratorAccessor<GRAPH>::edgesBegin(g);
            return const_iterator(iter,f);
        }
        static const_iterator
        end(const GRAPH & g,const MAP & map){
            ConstTransform f(map);
            typename  GRAPH::EdgeIt iter = GraphIteratorAccessor<GRAPH>::edgesEnd(g);
            return const_iterator(iter,f);
        }
    private:

    };


    template<class GRAPH,class MAP>
    class NodeMapIteratorHelper{
    public:
        typedef typename GraphMapTypeTraits<MAP>::Reference      Reference;
        typedef typename GraphMapTypeTraits<MAP>::ConstReference ConstReference;
        typedef typename GraphMapTypeTraits<MAP>::Value          Value;
    private:
        struct Transform{
            
            Transform(MAP & map)
            : map_(&map){
            }
            template<class ITEM>
            Reference operator()(const ITEM & item)const{
                return  map_->operator[](item);
            }
            mutable MAP * map_;
        };  
        struct ConstTransform{
            
            ConstTransform(const MAP & map)
            : map_(&map){
            }
            template<class ITEM>
            ConstReference operator()(const ITEM & item)const{
                return  map_->operator[](item);
            }
            const MAP * map_;
        }; 
    public:
        typedef  boost::transform_iterator< Transform,      typename GRAPH::NodeIt,Reference      ,Value> iterator;
        typedef  boost::transform_iterator< ConstTransform, typename GRAPH::NodeIt,ConstReference ,Value> const_iterator;
        static iterator
        begin(const GRAPH & g, MAP & map){
            Transform f(map);
            typename  GRAPH::NodeIt iter = GraphIteratorAccessor<GRAPH>::nodesBegin(g);
            return iterator(iter,f);
        }
        static iterator
        end(const GRAPH & g, MAP & map){
            Transform f(map);
            typename  GRAPH::NodeIt iter = GraphIteratorAccessor<GRAPH>::nodesEnd(g);
            return iterator(iter,f);
        }
        static const_iterator
        begin(const GRAPH & g, const MAP & map){
            ConstTransform f(map);
            typename  GRAPH::NodeIt iter = GraphIteratorAccessor<GRAPH>::nodesBegin(g);
            return const_iterator(iter,f);
        }
        static const_iterator
        end(const GRAPH & g,const MAP & map){
            ConstTransform f(map);
            typename  GRAPH::NodeIt iter = GraphIteratorAccessor<GRAPH>::nodesEnd(g);
            return const_iterator(iter,f);
        }
    private:

    };




    template<class GRAPH,class HYPER_EDGE_MAP,class HYPER_EDGE_SIZE_MAP>
    void hyperEdgeSizes(
        const GRAPH & g,
        const HYPER_EDGE_MAP & hyperEdges,
        HYPER_EDGE_SIZE_MAP  & out
    ){
        for(typename  GRAPH::EdgeIt e(g);e!=lemon::INVALID;++e){
            const size_t size = hyperEdges[*e].size();
            out[*e]=static_cast<typename HYPER_EDGE_SIZE_MAP::Value>(size);
        }
    }

    template<class RAG,class GRAPH,class NODE_LABEL_MAP,class HYPER_NODE_SIZE_MAP>
    void hyperNodeSizes(
        const RAG   & rag,
        const GRAPH & g,
        const NODE_LABEL_MAP & nodeLabelMap,
        HYPER_NODE_SIZE_MAP  & out
    ){
        typedef NodeMapIteratorHelper<GRAPH,NODE_LABEL_MAP> LabelIterHelper;
        typedef NodeMapIteratorHelper<RAG,HYPER_NODE_SIZE_MAP> OutIterHelper;
        typedef typename NODE_LABEL_MAP::Value LabelType;
        typedef typename HYPER_NODE_SIZE_MAP::Value SizeType;
        //std::fill(OutIterHelper::begin(rag,out),OutIterHelper::end(rag,out),static_cast<SizeType>(0));

        typename LabelIterHelper::const_iterator labelIter    = LabelIterHelper::begin(g,nodeLabelMap);
        typename LabelIterHelper::const_iterator labelIterEnd = LabelIterHelper::end(g,nodeLabelMap);

        std::vector<SizeType> labelCount(rag.maxNodeId()+1);
        while(labelIter!=labelIterEnd){
            const LabelType l = *labelIter;
            if(l<=rag.maxNodeId()){
                labelCount[l]+=static_cast<SizeType>(1);
            }
            ++labelIter;
        }
        for(typename RAG::NodeIt n(rag);n!=lemon::INVALID;++n){
            out[*n]=labelCount[rag.id(*n)];
        }
    }

    template<class RAG,class GRAPH,class NODE_LABEL_MAP,class IMAGE_MAP,class HYPER_NODE_FEATURE_MAP>
    void hyperNodeImageFeatures(
        const RAG   & rag,
        const GRAPH & g,
        const NODE_LABEL_MAP & nodeLabelMap,
        // if image map is const type there is error...
        IMAGE_MAP & imageMap,
        HYPER_NODE_FEATURE_MAP & out
    ){
        typedef NodeMapIteratorHelper<GRAPH,NODE_LABEL_MAP>       LabelIterHelper;
        typedef NodeMapIteratorHelper<RAG,HYPER_NODE_FEATURE_MAP> OutIterHelper;
        typedef typename NODE_LABEL_MAP::Value LabelType;
        std::fill(OutIterHelper::begin(rag,out),OutIterHelper::end(rag,out),0.0);
        std::vector<size_t> labelCount(rag.maxNodeId()+1);

        for(typename  GRAPH::NodeIt n(g);n!=lemon::INVALID;++n){
            const LabelType label = nodeLabelMap[*n];
            const typename RAG::Node ragNode = rag.nodeFromId(label);
            if(ragNode!=lemon::INVALID){
                labelCount[label]+=1;
                out[ragNode]+=imageMap[*n];
            }
        }
        for(typename  RAG::NodeIt n(rag);n!=lemon::INVALID;++n){
            out[*n]/=labelCount[rag.id(*n)];
        }    
    }

    template<class RAG,class GRAPH,class HYPER_EDGE_COORDINATE_MAP,class IMAGE,class HYPER_EDGE_FEATURE_MAP>
    void hyperEdgeImageFeatures(
        const RAG & rag,
        const GRAPH & graph,
        const HYPER_EDGE_COORDINATE_MAP & hyperEdges,
        const IMAGE & image,
        HYPER_EDGE_FEATURE_MAP  & out
    ){
        typedef typename HYPER_EDGE_COORDINATE_MAP::ConstReference HyperEdgeCoordVec;
        typedef typename HYPER_EDGE_COORDINATE_MAP::Value::value_type TinyVectorType;
        typedef HYPER_EDGE_FEATURE_MAP OutMap;
        typedef typename OutMap::Value Value;
        typedef typename IMAGE::difference_type ImageCoord;

        for(typename  RAG::EdgeIt e(rag);e!=lemon::INVALID;++e){
            HyperEdgeCoordVec hyperEdgeCoords = hyperEdges[*e];
            out[*e]=Value(0);
            const size_t nEdges = hyperEdgeCoords.size();
            for(size_t i=0;i<nEdges;++i){
                //TinyVectorType edgeCoord = hyperEdgeCoords[i];
                //const size_t oi=edgeCoord[edgeCoord.size()-1];

                // todo: replace me with morge "vigra-ish" functions
                //typedef typename IMAGE::difference_type DiffType;
                //DiffType imgCoord;
                //std::copy(edgeCoord.begin(),edgeCoord.begin()+imgCoord.size(),imgCoord.begin());
                out[*e]+=image[graph.u( hyperEdgeCoords[i]) ];
                out[*e]+=image[graph.v( hyperEdgeCoords[i]) ];
                //imgCoord[oi]+=1;
                //out[*e]+=image[imgCoord];
            }
            // todo: replace me with functors normalization / outpu
            out[*e]/=static_cast<Value>(2*hyperEdgeCoords.size());
        }
    }



    template<
        class GRAPH_IN,
        class GRAPH_IN_NODE_LABEL_MAP
    >
    void makeRegionAdjacencyGraph(
        GRAPH_IN                   graphIn,
        GRAPH_IN_NODE_LABEL_MAP    labels,
        AdjacencyListGraph & rag,
        typename AdjacencyListGraph:: template EdgeMap< std::vector<typename GRAPH_IN::Edge> > & hyperEdges,
        const Int64   ignoreLabel=-1
    ){
        typedef GRAPH_IN_NODE_LABEL_MAP LabelMap;
        typedef typename GraphMapTypeTraits<GRAPH_IN_NODE_LABEL_MAP>::Value LabelType;
        typedef GRAPH_IN GraphIn;
        typedef typename GraphIn::Edge EdgeGraphIn;
        typedef typename GraphIn::Node NodeGraphIn;
        typedef typename GraphIn::EdgeIt EdgeItInGraph;
        typedef typename GraphIn::NodeIt NodeItInGraph;
        
        typedef AdjacencyListGraph      GraphOut;
        typedef typename GraphOut::Edge EdgeGraphOut;
        typedef typename GraphOut::Node NodeGraphOut;
        typedef typename GraphOut::EdgeIt EdgeItOutGraph;
        typedef typename GraphOut::NodeIt NodeItOutGraph;
        typedef typename GraphOut:: template EdgeMap< std::vector<EdgeGraphIn> > HyperEdgeMap;
        typedef typename GraphOut:: template NodeMap< std::vector<NodeGraphIn> > HyperNodeMap;

        
        // iterate over all labels in the node map to find min max
        typedef NodeMapIteratorHelper<GraphIn,LabelMap> NodeIterHelper;
        const LabelType  minLabel = *std::min_element(NodeIterHelper::begin(graphIn,labels),NodeIterHelper::end(graphIn,labels));
        const LabelType  maxLabel = *std::max_element(NodeIterHelper::begin(graphIn,labels),NodeIterHelper::end(graphIn,labels));

        if(ignoreLabel==0 && rag.zeroStart())
            throw std::runtime_error("if ignoreLabel == 0, rag.zeroStart() must be false");
        if( minLabel==1 && rag.zeroStart() )
            throw std::runtime_error("if minLabel==1, rag.zeroStart() must be false");
        if( ignoreLabel==1  && !rag.zeroStart())
            throw std::runtime_error("if ignoreLabel == 1, rag.zerStart() must be true");
        if( ignoreLabel==1  && minLabel!=0)
            throw std::runtime_error("if ignoreLabel == 1, minLabel must be 0");


        if(ignoreLabel!=0 || minLabel!= 1 || rag.zeroStart()){
            throw std::runtime_error("WORK IN PROGRESS LIMITATION VIOLATED  => this should evaluate to false (ignoreLabel!=0 || minLabel!= 1 || rag.zeroStart() ) ");
        }



        size_t numberOfNodes = maxLabel; // TODO
        size_t reserveEdges  = 0;

        // SET UP RAG GAPPH
        // add nodes
        for(size_t n=0;n<numberOfNodes;++n){
            rag.addNode();
        }
        // add al edges
        for(EdgeItInGraph e(graphIn);e!=lemon::INVALID;++e){
            const EdgeGraphIn edge(*e);
            const LabelType lu = labels[graphIn.u(edge)];
            const LabelType lv = labels[graphIn.v(edge)];
            if(lu!=lv){
                // if there is an edge between lu and lv no new edge will be added
                rag.addEdge(rag.nodeFromId(lu),rag.nodeFromId(lv));
            }
        }

        // SET UP HYPEREDGES


        hyperEdges.assign(rag);
        // add edges
        for(EdgeItInGraph e(graphIn);e!=lemon::INVALID;++e){
            const EdgeGraphIn edge(*e);
            const LabelType lu = labels[graphIn.u(edge)];
            const LabelType lv = labels[graphIn.v(edge)];
            if(lu!=lv){
                EdgeGraphOut ragEdge= rag.findEdge(rag.nodeFromId(lu),rag.nodeFromId(lv));
                hyperEdges[ragEdge].push_back(edge);
            }
        }
    }
        
    template<class GRAPH,class WEIGHTS,class PREDECESSORS,class DISTANCE>
    void shortestPathDijkstra(
        const GRAPH         &           graph,
        const typename GRAPH::Node &    source,
        const WEIGHTS       &           weights,
        PREDECESSORS        &           predecessors,
        DISTANCE            &           distance,
        const typename GRAPH::Node &    target  = lemon::INVALID
    ){

        typedef GRAPH                       Graph;
        typedef typename Graph::Node        Node;
        typedef typename Graph::Edge        Edge;
        typedef typename Graph::NodeIt      NodeIt;
        typedef typename Graph::OutArcIt    OutArcIt;
        typedef typename WEIGHTS::value_type     WeightType;
        typedef typename DISTANCE::value_type    DistanceType;
        const size_t maxNodeId = graph.maxNodeId();
        vigra::ChangeablePriorityQueue<typename WEIGHTS::value_type> pq(maxNodeId);

        for(NodeIt n(graph);n!=lemon::INVALID;++n){
            const Node node(*n);
            pq.push(graph.id(node),std::numeric_limits<WeightType>::infinity() );
            distance[node]=std::numeric_limits<DistanceType>::infinity();
            predecessors[node]=lemon::INVALID;
        }

        distance[source]=static_cast<DistanceType>(0.0);
        pq.push(graph.id(source),0.0);

        bool finished=false;
        while(!pq.empty() && !finished){
            const WeightType minDist = pq.topPriority();
            if(minDist < std::numeric_limits<DistanceType>::infinity()){
                const Node topNode(graph.nodeFromId(pq.top()));
                pq.pop();
                // loop over all neigbours
                for(OutArcIt outArcIt(graph,topNode);outArcIt!=lemon::INVALID;++outArcIt){
                    const Node otherNode = graph.target(*outArcIt);
                    const size_t otherNodeId = graph.id(otherNode);

                    if(pq.contains(otherNodeId)){
                        const Edge edge(*outArcIt);
                        const DistanceType currentDist      = distance[otherNode];
                        const DistanceType alternativeDist  = distance[topNode]+weights[edge];
                        if(alternativeDist<currentDist){
                            pq.push(otherNodeId,alternativeDist);
                            distance[otherNode]=alternativeDist;
                            predecessors[otherNode]=topNode;
                        }
                    }
                    if(target==otherNode){
                        finished=true;
                        break;
                    }
                }
            }
            else{
                finished=true;
                break;
            }
            if(finished){
                break;
            }
        }
    }


    template<class GRAPH,class T>
    struct ZeroHeuristc{
        typedef typename GRAPH::Node Node;
        T operator()(const Node & a ,const Node & b )const{
            return static_cast<T>(0.0);
        }
    };


    template<class GRAPH,class WEIGHTS,class PREDECESSORS,class DISTANCE,class HEURSTIC>
    void shortestPathAStar(
        const GRAPH         &           graph,
        const typename GRAPH::Node &    source,
        const typename GRAPH::Node &    target,
        const WEIGHTS       &           weights,
        PREDECESSORS        &           predecessors,
        DISTANCE            &           distance,
        const HEURSTIC      &           heuristic
    ){

        typedef GRAPH                       Graph;
        typedef typename Graph::Node        Node;
        typedef typename Graph::Edge        Edge;
        typedef typename Graph::NodeIt      NodeIt;
        typedef typename Graph::OutArcIt    OutArcIt;

        typedef typename WEIGHTS::value_type     WeightType;
        typedef typename DISTANCE::value_type    DistanceType;

        typename  GRAPH:: template NodeMap<bool> closedSet(graph);
        vigra::ChangeablePriorityQueue<typename WEIGHTS::value_type> estimatedDistanceOpenSet(graph.maxNodeId());
        // initialize
        for(NodeIt n(graph);n!=lemon::INVALID;++n){
            const Node node(*n);
            closedSet[node]=false;
            distance[node]=std::numeric_limits<DistanceType>::infinity();
            predecessors[node]=lemon::INVALID;
        }
        // distance and estimated distance for start node
        distance[source]=static_cast<DistanceType>(0.0);
        estimatedDistanceOpenSet.push(graph.id(source),heuristic(source,target));

        // while any nodes left in openSet
        while(!estimatedDistanceOpenSet.empty()){

            // get the node with the lpwest estimated distance in the open set
            const Node current = graph.nodeFromId(estimatedDistanceOpenSet.top());

            // reached target?
            if(current==target)
                break;

            // remove current from openSet
            // add current to closedSet
            estimatedDistanceOpenSet.pop();
            closedSet[current]=true;

            // iterate over neigbours of current
            for(OutArcIt outArcIt(graph,current);outArcIt!=lemon::INVALID;++outArcIt){

                // get neigbour node and id
                const Node neighbour = graph.target(*outArcIt);
                const size_t neighbourId = graph.id(neighbour);

                // if neighbour is not yet in closedSet
                if(!closedSet[neighbour]){

                    // get edge between current and neigbour
                    const Edge edge(*outArcIt);

                    // get tentative score
                    const DistanceType tenativeScore = distance[current] + weights[edge];

                    // neighbour NOT in openSet OR tentative score better than the current distance
                    if(!estimatedDistanceOpenSet.contains(neighbourId) || tenativeScore < distance[neighbour] ){
                        // set predecessors and distance
                        predecessors[neighbour]=current;
                        distance[neighbour]=tenativeScore;

                        // update the estimated cost from neighbour to target
                        // ( and neigbour will be (re)-added to openSet)
                        estimatedDistanceOpenSet.push(neighbourId,distance[neighbour]+heuristic(neighbour,target));
                    }
                }
            }
        }
    }
      

} // namespace vigra

#endif // VIGRA_GRAPH_MAP_ALGORITHMS_HXX
