import vigra
import vigra.graphs as graphs
import pylab


f       = '100075.jpg'
#f       = '69015.jpg'
#f       = '12003.jpg'
sigma   = 1.0

print "prepare input"
img                 = vigra.impex.readImage(f)
imgLab              = vigra.colors.transform_RGB2Lab(img)
imgLabInterpolated  = vigra.resize(imgLab, [imgLab.shape[0]*2-1, imgLab.shape[1]*2-1])
gradmag             = vigra.filters.gaussianGradientMagnitude(imgLab, sigma)
gradmagInterpolated = vigra.filters.gaussianGradientMagnitude(imgLabInterpolated, sigma)
labels, nseg        = vigra.analysis.slicSuperpixels(imgLab, 20.0, 5)
labels              = vigra.analysis.labelImage(labels)

graph0, graph1 = graphs.gridRegionAdjacencyGraph(labels=labels, ignoreLabel=None)


# edge weights / node weights
graph0NodeWeights = gradmag
graph0EdgeWeights = graphs.edgeFeaturesFromInterpolatedImage(graph0, gradmagInterpolated)

graph1EdgeWeights = graph1.accumulateEdgeFeatures(graph0EdgeWeights)
graph1NodeWeights = graph1.accumulateNodeFeatures(graph0NodeWeights)


# generate seeds
seeds = graphs.nodeWeightedWatershedsSeeds(graph1, graph1NodeWeights)

# node weighted watersheds
labelsNodeWeighted  = graphs.nodeWeightedWatersheds(graph1, graph1NodeWeights, seeds)

# edge weighted watersheds
labelsEdgeWeighted  = graphs.edgeWeightedWatersheds(graph1, graph1EdgeWeights, seeds)


f = pylab.figure()
ax0 = f.add_subplot(1, 2, 0)
graph1.showNested(img, labelsNodeWeighted)
ax0.set_title("node weighted")

ax1 = f.add_subplot(1, 2, 1)
graph1.showNested(img, labelsEdgeWeighted)
ax1.set_title("edge weighted")
pylab.show()
