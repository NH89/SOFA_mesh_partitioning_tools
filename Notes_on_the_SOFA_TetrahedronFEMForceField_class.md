 

# Notes on the SOFA TetrahedronFEMForceField class



This is the general purpose large strain co-rotational FEM model. It includes plastic deformation and per-element local stiffness.

These can be accessed via the Python API to animate a simulation.
Internal attributes of the class

## protected:

    TetrahedronFEMForceField()

        : _mesh(NULL)

        , _indexedElements(NULL)

        , needUpdateTopology(false)

        , _initialPoints(initData(&_initialPoints, "initialPoints", "Initial Position"))

        , f_method(initData(&f_method,std::string("large"),"method","\"small\", \"large\" (by QR), \"polar\" or \"svd\" displacements"))

        , _poissonRatio(initData(&_poissonRatio,(Real)0.45f,"poissonRatio","FEM Poisson Ratio [0,0.5["))

        , _youngModulus(initData(&_youngModulus,"youngModulus","FEM Young Modulus"))

        , _localStiffnessFactor(initData(&_localStiffnessFactor, "localStiffnessFactor","Allow specification of different stiffness per element. If there are N element and M values are specified, the youngModulus factor for element i would be localStiffnessFactor[i*M/N]"))

        , _updateStiffnessMatrix(initData(&_updateStiffnessMatrix,false,"updateStiffnessMatrix",""))

        , _assembling(initData(&_assembling,false,"computeGlobalMatrix",""))

        , _plasticMaxThreshold(initData(&_plasticMaxThreshold,(Real)0.f,"plasticMaxThreshold","Plastic Max Threshold (2-norm of the strain)"))

        , _plasticYieldThreshold(initData(&_plasticYieldThreshold,(Real)0.0001f,"plasticYieldThreshold","Plastic Yield Threshold (2-norm of the strain)"))

        , _plasticCreep(initData(&_plasticCreep,(Real)0.9f,"plasticCreep","Plastic Creep Factor * dt [0,1]. Warning this factor depends on dt."))

        , _gatherPt(initData(&_gatherPt,"gatherPt","number of dof accumulated per threads during the gather operation (Only use in GPU version)"))

        , _gatherBsize(initData(&_gatherBsize,"gatherBsize","number of dof accumulated per threads during the gather operation (Only use in GPU version)"))

        , drawHeterogeneousTetra(initData(&drawHeterogeneousTetra,false,"drawHeterogeneousTetra","Draw Heterogeneous Tetra in different color"))

        , drawAsEdges(initData(&drawAsEdges,false,"drawAsEdges","Draw as edges instead of tetrahedra"))

        , _computeVonMisesStress(initData(&_computeVonMisesStress,0,"computeVonMisesStress","compute and display von Mises stress: 0: no computations, 1: using corotational strain, 2: using full Green strain"))

        , _vonMisesPerElement(initData(&_vonMisesPerElement, "vonMisesPerElement", "von Mises Stress per element"))

        , _vonMisesPerNode(initData(&_vonMisesPerNode, "vonMisesPerNode", "von Mises Stress per node"))

        , _vonMisesStressColors(initData(&_vonMisesStressColors, "vonMisesStressColors", "Vector of colors describing the VonMises stress"))

#ifdef SOFATETRAHEDRONFEMFORCEFIELD_COLORMAP

        , _showStressColorMap(initData(&_showStressColorMap,"showStressColorMap", "Color map used to show stress values"))

        , _showStressAlpha(initData(&_showStressAlpha, 1.0f, "showStressAlpha", "Alpha for vonMises visualisation"))

        , _showVonMisesStressPerNode(initData(&_showVonMisesStressPerNode,false,"showVonMisesStressPerNode","draw points  showing vonMises stress interpolated in nodes"))

#endif

        , isToPrint( initData(&isToPrint, false, "isToPrint", "suppress somes data before using save as function"))

        , _updateStiffness(initData(&_updateStiffness,false,"updateStiffness","udpate structures (precomputed in init) using stiffness parameters in each iteration (set listening=1)"))

    {

        _poissonRatio.setRequired(true);

        _youngModulus.setRequired(true);

        _youngModulus.beginEdit()->push_back((Real)5000.);

        _youngModulus.endEdit();

        _youngModulus.unset();


        data.initPtrData(this);

        this->addAlias(&_assembling, "assembling");

        minYoung = 0.0;

        maxYoung = 0.0;

