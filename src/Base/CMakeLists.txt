if(WIN32)
    add_definitions(-DFCBase)
    add_definitions(-DPYCXX_DLL)
    add_definitions(-DBOOST_DYN_LINK)
endif(WIN32)

include_directories(
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Mod/Part/App
    ${CMAKE_CURRENT_SOURCE_DIR}/../Mod/Part/Gui
    ${CMAKE_CURRENT_SOURCE_DIR}/../App
    ${CMAKE_CURRENT_SOURCE_DIR}/../Gui
    ${CMAKE_CURRENT_BINARY_DIR}/../Base
    ${CMAKE_CURRENT_BINARY_DIR}/../Gui
    ${CMAKE_CURRENT_BINARY_DIR}/../Gui/DAGView
    ${CMAKE_CURRENT_BINARY_DIR}/../Gui/TaskView
    ${CMAKE_CURRENT_BINARY_DIR}/../Gui/Language
    ${CMAKE_CURRENT_BINARY_DIR}/../Gui/propertyeditor
    ${CMAKE_CURRENT_BINARY_DIR}/../Mod/Part/Gui
    ${CMAKE_CURRENT_BINARY_DIR}/__/Mod/Part/Gui
    ${CMAKE_CURRENT_SOURCE_DIR}/__/Mod/Part/App
    ${CMAKE_CURRENT_SOURCE_DIR}/__/Mod/Part/Gui
    ${CMAKE_CURRENT_SOURCE_DIR}/__/App
    ${CMAKE_CURRENT_SOURCE_DIR}/__/Gui
    ${CMAKE_CURRENT_BINARY_DIR}/__/Base
    ${CMAKE_CURRENT_BINARY_DIR}/__/Gui
    ${CMAKE_CURRENT_BINARY_DIR}/__/Gui/DAGView
    ${CMAKE_CURRENT_BINARY_DIR}/__/Gui/TaskView
    ${CMAKE_CURRENT_BINARY_DIR}/__/Gui/Language
    ${CMAKE_CURRENT_BINARY_DIR}/__/Gui/propertyeditor
    ${CMAKE_CURRENT_BINARY_DIR}/__/Mod/Part/Gui
    ${OCC_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    ${XercesC_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
    ${PYCXX_INCLUDE_DIR}
)

link_directories(${OCC_LIBRARY_DIR})

if(MSVC)
    set(FreeCADBase_LIBS
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARIES}
        debug ${XercesC_DEBUG_LIBRARIES}
        optimized ${XercesC_LIBRARIES}
        ${ZLIB_LIBRARIES}
        debug MSVCRTD.LIB
        debug MSVCPRTD.LIB
        optimized MSVCRT.LIB
        optimized MSVCPRT.LIB
        Rpcrt4.lib
        version.lib
    )

    #Universal C runtime introduced in VS 2015 (cl version 19)
    if (NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19"))
        list(APPEND FreeCADBase_LIBS
            debug vcruntimed.lib
            debug ucrtd.lib
            optimized vcruntime.lib
            optimized ucrt.lib
        )
    endif()
elseif(MINGW)
    set(FreeCADBase_LIBS
        ${PYTHON_LIBRARIES}
        ${XercesC_LIBRARIES}
        ${Boost_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${OCC_LIBRARIES}
        ${OCC_DEBUG_LIBRARIES}
        Rpcrt4.lib
    )
else(MSVC)
    set(FreeCADBase_LIBS
        ${COIN3D_LIBRARIES}
        ${Boost_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${XercesC_LIBRARIES}
        ${Boost_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${OCC_LIBRARIES}
        ${OCC_DEBUG_LIBRARIES}
        IfcParse
        IfcGeom
        icui18n
        icuuc
        icudata
    )
endif(MSVC)

if (BUILD_QT5)
    include_directories(
        ${Qt5Core_INCLUDE_DIRS}
    )
    list(APPEND FreeCADBase_LIBS ${Qt5Core_LIBRARIES})
else()
    include_directories(
        ${QT_QTCORE_INCLUDE_DIR}
    )
    list(APPEND FreeCADBase_LIBS ${QT_LIBRARIES})
endif()

# needed for OpenSUSE
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND FreeCADBase_LIBS -lutil -ldl)
endif()

generate_from_xml(BaseClassPy)
generate_from_xml(BoundBoxPy)
generate_from_xml(CoordinateSystemPy)
generate_from_xml(PersistencePy)
generate_from_xml(VectorPy)
generate_from_xml(MatrixPy)
generate_from_xml(RotationPy)
generate_from_xml(PlacementPy)
generate_from_xml(AxisPy)
generate_from_xml(UnitPy)
generate_from_xml(QuantityPy)

if(SWIG_FOUND)
    execute_process(COMMAND ${SWIG_EXECUTABLE} -python -external-runtime ${CMAKE_CURRENT_BINARY_DIR}/swigpyrun.h)
    add_definitions(-DHAVE_SWIG=1)
endif(SWIG_FOUND)

if (EXISTS ${CMAKE_SOURCE_DIR}/src/zipios++ AND NOT FREECAD_USE_EXTERNAL_ZIPIOS)
SET(zipios_SRCS
    ../zipios++/backbuffer.h
    ../zipios++/basicentry.cpp
    ../zipios++/basicentry.h
    ../zipios++/collcoll.cpp
    ../zipios++/collcoll.h
    ../zipios++/deflateoutputstreambuf.cpp
    ../zipios++/deflateoutputstreambuf.h
    ../zipios++/dircoll.cpp
    ../zipios++/dircoll.h
    ../zipios++/directory.cpp
    ../zipios++/directory.h
    ../zipios++/fcoll.cpp
    ../zipios++/fcoll.h
    ../zipios++/fcollexceptions.cpp
    ../zipios++/fcollexceptions.h
    ../zipios++/fileentry.cpp
    ../zipios++/fileentry.h
    ../zipios++/filepath.cpp
    ../zipios++/filepath.h
    ../zipios++/filterinputstreambuf.cpp
    ../zipios++/filterinputstreambuf.h
    ../zipios++/filteroutputstreambuf.cpp
    ../zipios++/filteroutputstreambuf.h
    ../zipios++/gzipoutputstream.cpp
    ../zipios++/gzipoutputstream.h
    ../zipios++/gzipoutputstreambuf.cpp
    ../zipios++/gzipoutputstreambuf.h
    ../zipios++/inflateinputstreambuf.cpp
    ../zipios++/inflateinputstreambuf.h
    ../zipios++/meta-iostreams.h
    ../zipios++/outputstringstream.h
    ../zipios++/simplesmartptr.h
    ../zipios++/virtualseeker.h
    ../zipios++/zipfile.cpp
    ../zipios++/zipfile.h
    ../zipios++/ziphead.cpp
    ../zipios++/ziphead.h
    ../zipios++/zipheadio.cpp
    ../zipios++/zipheadio.h
    ../zipios++/zipinputstream.cpp
    ../zipios++/zipinputstream.h
    ../zipios++/zipinputstreambuf.cpp
    ../zipios++/zipinputstreambuf.h
    ../zipios++/zipios_common.h
    ../zipios++/zipios-config.h
    ../zipios++/zipios_defs.h
    ../zipios++/zipoutputstreambuf.cpp
    ../zipios++/zipoutputstreambuf.h
    ../zipios++/zipoutputstream.cpp
    ../zipios++/zipoutputstream.h
)
SOURCE_GROUP("zipios" FILES ${zipios_SRCS})
endif ()

SOURCE_GROUP("pycxx" FILES ${PYCXX_SOURCES})

SET(FreeCADBase_XML_SRCS
    AxisPy.xml
    BaseClassPy.xml
    BoundBoxPy.xml
    CoordinateSystemPy.xml
    MatrixPy.xml
    PersistencePy.xml
    PlacementPy.xml
    RotationPy.xml
    VectorPy.xml
    QuantityPy.xml
    UnitPy.xml
)
SOURCE_GROUP("XML" FILES ${FreeCADBase_XML_SRCS})

set(FreeCADBase_MOC_HDRS
    Debugger.h
    FutureWatcherProgress.h
)

fc_wrap_cpp(FreeCADBase_MOC_SRCS ${FreeCADBase_MOC_HDRS})

SET(FreeCADBase_UNITAPI_SRCS
    UnitsApi.cpp
    UnitsApiPy.cpp
    UnitsApi.h
    UnitsSchema.h
    UnitsSchema.cpp
    UnitsSchemaInternal.h
    UnitsSchemaInternal.cpp
    UnitsSchemaMKS.h
    UnitsSchemaMKS.cpp
    UnitsSchemaImperial1.h
    UnitsSchemaImperial1.cpp
    UnitsSchemaCentimeters.h
    UnitsSchemaCentimeters.cpp
    Quantity.h
    Quantity.cpp
    QuantityPyImp.cpp
    QuantityParser.l
    QuantityParser.y
    Unit.h
    Unit.cpp
    UnitPyImp.cpp
    
)
SOURCE_GROUP("Units" FILES ${FreeCADBase_UNITAPI_SRCS})

if(PYTHON_VERSION_MAJOR LESS 3)
SET(SWIG_SRCS
    swigpyrun_1.3.25.cpp
    swigpyrun_1.3.33.cpp
    swigpyrun_1.3.36.cpp
    swigpyrun_1.3.38.cpp
    swigpyrun_1.3.40.cpp
    swigpyrun.cpp
)
else(PYTHON_VERSION_MAJOR LESS 3)
SET(SWIG_SRCS
    swigpyrun.cpp
)
endif(PYTHON_VERSION_MAJOR LESS 3)

SET(FreeCADBase_CPP_SRCS
    Axis.cpp
    AxisPyImp.cpp
    Base64.cpp
    BaseClass.cpp
    BaseClassPyImp.cpp
    BoundBoxPyImp.cpp
    Builder3D.cpp
    Console.cpp
    CoordinateSystem.cpp
    CoordinateSystemPyImp.cpp
    Debugger.cpp
    Exception.cpp
    ExceptionFactory.cpp
    Factory.cpp
    FileInfo.cpp
    FileTemplate.cpp
    FutureWatcherProgress.cpp
    gzstream.cpp
    gzstream.h
    GeometryPyCXX.cpp
    Handle.cpp
    InputSource.cpp
    Interpreter.cpp
    Matrix.cpp
    MatrixPyImp.cpp
    MemDebug.cpp
    Parameter.cpp
    ParameterPy.cpp
    Persistence.cpp
    PersistencePyImp.cpp
    Placement.cpp
    PlacementPyImp.cpp
    PyExport.cpp
    PyObjectBase.cpp
    Reader.cpp
    Rotation.cpp
    RotationPyImp.cpp
    Sequencer.cpp
    Stream.cpp
    Swap.cpp
    ${SWIG_SRCS}
    TimeInfo.cpp
    Tools.cpp
    Tools2D.cpp
    Type.cpp
    Uuid.cpp
    Vector3D.cpp
    VectorPyImp.cpp
    Writer.cpp
    XMLTools.cpp
)

if(PYTHON_VERSION_MAJOR LESS 3)
SET(SWIG_HEADERS
    swigpyrun_1.3.25.h
    swigpyrun_1.3.33.h
    swigpyrun_1.3.36.h
    swigpyrun_1.3.38.h
    swigpyrun_1.3.40.h
    swigpyrun.inl
)
else(PYTHON_VERSION_MAJOR LESS 3)
SET(SWIG_HEADERS
    swigpyrun.inl
)
endif(PYTHON_VERSION_MAJOR LESS 3)

SET(FreeCADBase_HPP_SRCS
    Axis.h
    Base64.h
    BaseClass.h
    BoundBox.h
    Builder3D.h
    Console.h
    CoordinateSystem.h
    Debugger.h
    Exception.h
    ExceptionFactory.h
    Factory.h
    FileInfo.h
    FileTemplate.h
    FutureWatcherProgress.h
    fdstream.hpp
    gzstream.h
    GeometryPyCXX.h
    Handle.h
    InputSource.h
    Interpreter.h
    Matrix.h
    MemDebug.h
    Observer.h
    Parameter.h
    Persistence.h
    Placement.h
    PyExport.h
    PyObjectBase.h
    Reader.h
    Rotation.h
    Sequencer.h
    Stream.h
    Swap.h
    ${SWIG_HEADERS}
    TimeInfo.h
    Tools.h
    Tools2D.h
    Type.h
    Uuid.h
    Vector3D.h
    ViewProj.h
    Writer.h
    XMLTools.h
)
SET(Features_SRCS
    ../Mod/Part/App/FeaturePartBoolean.cpp
    ../Mod/Part/App/FeaturePartBoolean.h
    ../Mod/Part/App/FeaturePartBox.cpp
    ../Mod/Part/App/FeaturePartBox.h
    ../Mod/Part/App/FeaturePartCircle.cpp
    ../Mod/Part/App/FeaturePartCircle.h
    ../Mod/Part/App/FeaturePartCommon.cpp
    ../Mod/Part/App/FeaturePartCommon.h
    ../Mod/Part/App/FeaturePartCurveNet.cpp
    ../Mod/Part/App/FeaturePartCurveNet.h
    ../Mod/Part/App/FeaturePartCut.cpp
    ../Mod/Part/App/FeaturePartCut.h
    ../Mod/Part/App/FeaturePartFuse.cpp
    ../Mod/Part/App/FeaturePartFuse.h
    ../Mod/Part/App/FeaturePartImportBrep.cpp
    ../Mod/Part/App/FeaturePartImportBrep.h
    ../Mod/Part/App/FeaturePartImportIges.cpp
    ../Mod/Part/App/FeaturePartImportIges.h
    ../Mod/Part/App/FeaturePartImportStep.cpp
    ../Mod/Part/App/FeaturePartImportStep.h
    ../Mod/Part/App/FeaturePartPolygon.cpp
    ../Mod/Part/App/FeaturePartPolygon.h
    ../Mod/Part/App/FeaturePartSection.cpp
    ../Mod/Part/App/FeaturePartSection.h
    ../Mod/Part/App/FeaturePartSpline.cpp
    ../Mod/Part/App/FeaturePartSpline.h
    ../Mod/Part/App/FeatureChamfer.cpp
    ../Mod/Part/App/FeatureChamfer.h
    ../Mod/Part/App/FeatureCompound.cpp
    ../Mod/Part/App/FeatureCompound.h
    ../Mod/Part/App/FeatureExtrusion.cpp
    ../Mod/Part/App/FeatureExtrusion.h
    ../Mod/Part/App/FeatureFace.cpp
    ../Mod/Part/App/FeatureFace.h
    ../Mod/Part/App/FeatureFillet.cpp
    ../Mod/Part/App/FeatureFillet.h
    ../Mod/Part/App/FeatureMirroring.cpp
    ../Mod/Part/App/FeatureMirroring.h
    ../Mod/Part/App/FeatureRevolution.cpp
    ../Mod/Part/App/FeatureRevolution.h
    ../Mod/Part/App/FeatureOffset.cpp
    ../Mod/Part/App/FeatureOffset.h
    ../Mod/Part/App/PartFeatures.cpp
    ../Mod/Part/App/PartFeatures.h
    ../Mod/Part/App/PartFeature.cpp
    ../Mod/Part/App/PartFeature.h
    ../Mod/Part/App/PartFeatureReference.cpp
    ../Mod/Part/App/PartFeatureReference.h
    ../Mod/Part/App/Part2DObject.cpp
    ../Mod/Part/App/Part2DObject.h
    ../Mod/Part/App/PrimitiveFeature.cpp
    ../Mod/Part/App/PrimitiveFeature.h
    ../Mod/Part/App/FeatureGeometrySet.h
    ../Mod/Part/App/FeatureGeometrySet.cpp
    ../Mod/Part/App/CustomFeature.cpp
    ../Mod/Part/App/CustomFeature.h
    ../Mod/Part/App/BodyBase.h
    ../Mod/Part/App/BodyBase.cpp
    ../Mod/Part/App/DatumFeature.cpp
    ../Mod/Part/App/DatumFeature.h
    ../Mod/Part/App/AttachExtension.h
    ../Mod/Part/App/AttachExtension.cpp
)
SET(Properties_SRCS
    ../Mod/Part/App/PropertyTopoShape.cpp
    ../Mod/Part/App/PropertyTopoShape.h
    ../Mod/Part/App/PropertyGeometryList.cpp
    ../Mod/Part/App/PropertyGeometryList.h
)

SET(Part_SRCS
    ${Features_SRCS}
    ${Properties_SRCS}
#    ${Python_SRCS}
#    ${Geom2dPy_SRCS}
    ../Mod/Part/App/Attacher.cpp
    ../Mod/Part/App/Attacher.h
    ../Mod/Part/App/AppPart.cpp
    ../Mod/Part/App/AppPartPy.cpp
    ../Mod/Part/App/BSplineCurveBiArcs.cpp
    ../Mod/Part/App/CrossSection.cpp
    ../Mod/Part/App/CrossSection.h
    ../Mod/Part/App/Geometry.cpp
    ../Mod/Part/App/Geometry.h
    ../Mod/Part/App/Geometry2d.cpp
    ../Mod/Part/App/Geometry2d.h
    ../Mod/Part/App/ImportIges.cpp
    ../Mod/Part/App/ImportIges.h
    ../Mod/Part/App/ImportStep.cpp
    ../Mod/Part/App/ImportStep.h
    ../Mod/Part/App/PreCompiled.cpp
    ../Mod/Part/App/PreCompiled.h
    ../Mod/Part/App/ProgressIndicator.cpp
    ../Mod/Part/App/ProgressIndicator.h
    ../Mod/Part/App/TopoShape.cpp
    ../Mod/Part/App/TopoShape.h
    ../Mod/Part/App/edgecluster.cpp
    ../Mod/Part/App/edgecluster.h
    ../Mod/Part/App/modelRefine.cpp
    ../Mod/Part/App/modelRefine.h
    ../Mod/Part/App/Tools.cpp
    ../Mod/Part/App/Tools.h
    ../Mod/Part/App/encodeFilename.h
    ../Mod/Part/App/OCCError.h
    ../Mod/Part/App/FT2FC.cpp
    ../Mod/Part/App/FT2FC.h
    ../Mod/Part/App/FaceMaker.cpp
    ../Mod/Part/App/FaceMaker.h
    ../Mod/Part/App/FaceMakerCheese.cpp
    ../Mod/Part/App/FaceMakerCheese.h
    ../Mod/Part/App/FaceMakerBullseye.cpp
    ../Mod/Part/App/FaceMakerBullseye.h
)

set(PartGui_MOC_HDRS
    ../Mod/Part/Gui/CrossSections.h
    ../Mod/Part/Gui/Mirroring.h
    ../Mod/Part/Gui/DlgBooleanOperation.h
    ../Mod/Part/Gui/DlgExtrusion.h
    ../Mod/Part/Gui/DlgFilletEdges.h
    ../Mod/Part/Gui/DlgPartBoxImp.h
    ../Mod/Part/Gui/DlgPartCylinderImp.h
    ../Mod/Part/Gui/DlgPartImportIgesImp.h
    ../Mod/Part/Gui/DlgPartImportStepImp.h
    ../Mod/Part/Gui/DlgPrimitives.h
    ../Mod/Part/Gui/DlgRevolution.h
    ../Mod/Part/Gui/DlgSettings3DViewPartImp.h
    ../Mod/Part/Gui/DlgSettingsGeneral.h
    ../Mod/Part/Gui/DlgSettingsObjectColor.h
    ../Mod/Part/Gui/TaskFaceColors.h
    ../Mod/Part/Gui/TaskShapeBuilder.h
    ../Mod/Part/Gui/TaskLoft.h
    ../Mod/Part/Gui/TaskOffset.h
    ../Mod/Part/Gui/TaskSweep.h
    ../Mod/Part/Gui/TaskThickness.h
    ../Mod/Part/Gui/TaskDimension.h
    ../Mod/Part/Gui/TaskCheckGeometry.h
    ../Mod/Part/Gui/TaskAttacher.h
)
fc_wrap_cpp(PartGui_MOC_SRCS ${PartGui_MOC_HDRS})
SOURCE_GROUP("Moc" FILES ${PartGui_MOC_SRCS})

if (BUILD_QT5)
    qt5_add_resources(PartGui_QRC_SRCS ../Mod/Part/Gui/Resources/Part.qrc)
else()
    qt4_add_resources(PartGui_QRC_SRCS ../Mod/Part/Gui/Resources/Part.qrc)
endif()

set(PartGui_UIC_SRCS
    ../Mod/Part/Gui/CrossSections.ui
    ../Mod/Part/Gui/Mirroring.ui
    ../Mod/Part/Gui/DlgBooleanOperation.ui
    ../Mod/Part/Gui/DlgExtrusion.ui
    ../Mod/Part/Gui/DlgFilletEdges.ui
    ../Mod/Part/Gui/DlgImportExportIges.ui
    ../Mod/Part/Gui/DlgImportExportStep.ui
    ../Mod/Part/Gui/DlgPartBox.ui
    ../Mod/Part/Gui/DlgPartCylinder.ui
    ../Mod/Part/Gui/DlgPartImportIges.ui
    ../Mod/Part/Gui/DlgPartImportStep.ui
    ../Mod/Part/Gui/DlgPrimitives.ui
    ../Mod/Part/Gui/Location.ui
    ../Mod/Part/Gui/DlgRevolution.ui
    ../Mod/Part/Gui/DlgSettings3DViewPart.ui
    ../Mod/Part/Gui/DlgSettingsGeneral.ui
    ../Mod/Part/Gui/DlgSettingsObjectColor.ui
    ../Mod/Part/Gui/TaskFaceColors.ui
    ../Mod/Part/Gui/TaskShapeBuilder.ui
    ../Mod/Part/Gui/TaskLoft.ui
    ../Mod/Part/Gui/TaskOffset.ui
    ../Mod/Part/Gui/TaskSweep.ui
    ../Mod/Part/Gui/TaskAttacher.ui
)

if (BUILD_QT5)
    qt5_wrap_ui(PartGui_UIC_HDRS ${PartGui_UIC_SRCS})
else()
    qt4_wrap_ui(PartGui_UIC_HDRS ${PartGui_UIC_SRCS})
endif()


SET(PartGui_SRCS
    ${PartGui_QRC_SRCS}
    ${PartGui_UIC_HDRS}
    ../Mod/Part/Gui/AppPartGui.cpp
    ../Mod/Part/Gui/AttacherTexts.h
    ../Mod/Part/Gui/AttacherTexts.cpp
    ../Mod/Part/Gui/Command.cpp
    ../Mod/Part/Gui/CommandSimple.cpp
    ../Mod/Part/Gui/CommandParametric.cpp
    ../Mod/Part/Gui/CrossSections.cpp
    ../Mod/Part/Gui/CrossSections.h
    ../Mod/Part/Gui/CrossSections.ui
    ../Mod/Part/Gui/Mirroring.cpp
    ../Mod/Part/Gui/Mirroring.h
    ../Mod/Part/Gui/Mirroring.ui
    ../Mod/Part/Gui/DlgBooleanOperation.cpp
    ../Mod/Part/Gui/DlgBooleanOperation.h
    ../Mod/Part/Gui/DlgBooleanOperation.ui
    ../Mod/Part/Gui/DlgExtrusion.cpp
    ../Mod/Part/Gui/DlgExtrusion.h
    ../Mod/Part/Gui/DlgExtrusion.ui
    ../Mod/Part/Gui/DlgFilletEdges.cpp
    ../Mod/Part/Gui/DlgFilletEdges.h
    ../Mod/Part/Gui/DlgFilletEdges.ui
    ../Mod/Part/Gui/DlgImportExportIges.ui
    ../Mod/Part/Gui/DlgImportExportStep.ui
    ../Mod/Part/Gui/DlgPartBox.ui
    ../Mod/Part/Gui/DlgPartBoxImp.cpp
    ../Mod/Part/Gui/DlgPartBoxImp.h
    ../Mod/Part/Gui/DlgPartCylinderImp.cpp
    ../Mod/Part/Gui/DlgPartCylinderImp.h
    ../Mod/Part/Gui/DlgPartCylinder.ui
    ../Mod/Part/Gui/DlgPartImportIges.ui
    ../Mod/Part/Gui/DlgPartImportIgesImp.cpp
    ../Mod/Part/Gui/DlgPartImportIgesImp.h
    ../Mod/Part/Gui/DlgPartImportStep.ui
    ../Mod/Part/Gui/DlgPartImportStepImp.cpp
    ../Mod/Part/Gui/DlgPartImportStepImp.h
    ../Mod/Part/Gui/DlgPrimitives.cpp
    ../Mod/Part/Gui/DlgPrimitives.h
    ../Mod/Part/Gui/DlgPrimitives.ui
    ../Mod/Part/Gui/Location.ui
    ../Mod/Part/Gui/DlgRevolution.cpp
    ../Mod/Part/Gui/DlgRevolution.h
    ../Mod/Part/Gui/DlgRevolution.ui
    ../Mod/Part/Gui/DlgSettings3DViewPart.ui
    ../Mod/Part/Gui/DlgSettings3DViewPartImp.cpp
    ../Mod/Part/Gui/DlgSettings3DViewPartImp.h
    ../Mod/Part/Gui/DlgSettingsGeneral.cpp
    ../Mod/Part/Gui/DlgSettingsGeneral.h
    ../Mod/Part/Gui/DlgSettingsGeneral.ui
    ../Mod/Part/Gui/DlgSettingsObjectColor.cpp
    ../Mod/Part/Gui/DlgSettingsObjectColor.h
    ../Mod/Part/Gui/DlgSettingsObjectColor.ui
    ../Mod/Part/Gui/Resources/Part.qrc
    ../Mod/Part/Gui/PreCompiled.cpp
    ../Mod/Part/Gui/PreCompiled.h
    ../Mod/Part/Gui/SoFCShapeObject.cpp
    ../Mod/Part/Gui/SoFCShapeObject.h
    ../Mod/Part/Gui/SoBrepEdgeSet.cpp
    ../Mod/Part/Gui/SoBrepEdgeSet.h
    ../Mod/Part/Gui/SoBrepFaceSet.cpp
    ../Mod/Part/Gui/SoBrepFaceSet.h
    ../Mod/Part/Gui/SoBrepPointSet.cpp
    ../Mod/Part/Gui/SoBrepPointSet.h
    ../Mod/Part/Gui/ViewProvider.cpp
    ../Mod/Part/Gui/ViewProvider.h
    ../Mod/Part/Gui/ViewProviderExt.cpp
    ../Mod/Part/Gui/ViewProviderExt.h
    ../Mod/Part/Gui/ViewProviderReference.cpp
    ../Mod/Part/Gui/ViewProviderReference.h
    ../Mod/Part/Gui/ViewProviderBox.cpp
    ../Mod/Part/Gui/ViewProviderBox.h
    ../Mod/Part/Gui/ViewProviderCompound.cpp
    ../Mod/Part/Gui/ViewProviderCompound.h
    ../Mod/Part/Gui/ViewProviderCircleParametric.cpp
    ../Mod/Part/Gui/ViewProviderCircleParametric.h
    ../Mod/Part/Gui/ViewProviderLineParametric.cpp
    ../Mod/Part/Gui/ViewProviderLineParametric.h
    ../Mod/Part/Gui/ViewProviderPointParametric.cpp
    ../Mod/Part/Gui/ViewProviderPointParametric.h
    ../Mod/Part/Gui/ViewProviderEllipseParametric.cpp
    ../Mod/Part/Gui/ViewProviderEllipseParametric.h
    ../Mod/Part/Gui/ViewProviderHelixParametric.cpp
    ../Mod/Part/Gui/ViewProviderHelixParametric.h
    ../Mod/Part/Gui/ViewProviderPlaneParametric.cpp
    ../Mod/Part/Gui/ViewProviderPlaneParametric.h
    ../Mod/Part/Gui/ViewProviderSphereParametric.cpp
    ../Mod/Part/Gui/ViewProviderSphereParametric.h
    ../Mod/Part/Gui/ViewProviderCylinderParametric.cpp
    ../Mod/Part/Gui/ViewProviderCylinderParametric.h
    ../Mod/Part/Gui/ViewProviderConeParametric.cpp
    ../Mod/Part/Gui/ViewProviderConeParametric.h
    ../Mod/Part/Gui/ViewProviderPrism.cpp
    ../Mod/Part/Gui/ViewProviderPrism.h
    ../Mod/Part/Gui/ViewProviderRegularPolygon.cpp
    ../Mod/Part/Gui/ViewProviderRegularPolygon.h
    ../Mod/Part/Gui/ViewProviderTorusParametric.cpp
    ../Mod/Part/Gui/ViewProviderTorusParametric.h
    ../Mod/Part/Gui/ViewProviderCurveNet.cpp
    ../Mod/Part/Gui/ViewProviderCurveNet.h
    ../Mod/Part/Gui/ViewProviderSpline.cpp
    ../Mod/Part/Gui/ViewProviderSpline.h
    ../Mod/Part/Gui/ViewProviderImport.cpp
    ../Mod/Part/Gui/ViewProviderImport.h
    ../Mod/Part/Gui/ViewProviderExtrusion.cpp
    ../Mod/Part/Gui/ViewProviderExtrusion.h
    ../Mod/Part/Gui/ViewProvider2DObject.cpp
    ../Mod/Part/Gui/ViewProvider2DObject.h
    ../Mod/Part/Gui/ViewProviderMirror.cpp
    ../Mod/Part/Gui/ViewProviderMirror.h
    ../Mod/Part/Gui/ViewProviderPython.cpp
    ../Mod/Part/Gui/ViewProviderPython.h
    ../Mod/Part/Gui/ViewProviderBoolean.cpp
    ../Mod/Part/Gui/ViewProviderBoolean.h
    ../Mod/Part/Gui/ViewProviderRuledSurface.cpp
    ../Mod/Part/Gui/ViewProviderRuledSurface.h
    ../Mod/Part/Gui/Workbench.cpp
    ../Mod/Part/Gui/Workbench.h
    ../Mod/Part/Gui/TaskFaceColors.cpp
    ../Mod/Part/Gui/TaskFaceColors.h
    ../Mod/Part/Gui/TaskFaceColors.ui
    ../Mod/Part/Gui/TaskShapeBuilder.cpp
    ../Mod/Part/Gui/TaskShapeBuilder.h
    ../Mod/Part/Gui/TaskShapeBuilder.ui
    ../Mod/Part/Gui/TaskLoft.cpp
    ../Mod/Part/Gui/TaskLoft.h
    ../Mod/Part/Gui/TaskLoft.ui
    ../Mod/Part/Gui/TaskOffset.cpp
    ../Mod/Part/Gui/TaskOffset.h
    ../Mod/Part/Gui/TaskOffset.ui
    ../Mod/Part/Gui/TaskSweep.cpp
    ../Mod/Part/Gui/TaskSweep.h
    ../Mod/Part/Gui/TaskSweep.ui
    ../Mod/Part/Gui/TaskThickness.cpp
    ../Mod/Part/Gui/TaskThickness.h
    ../Mod/Part/Gui/TaskDimension.cpp
    ../Mod/Part/Gui/TaskDimension.h
    ../Mod/Part/Gui/TaskCheckGeometry.cpp
    ../Mod/Part/Gui/TaskCheckGeometry.h
    ../Mod/Part/Gui/TaskAttacher.h 
    ../Mod/Part/Gui/TaskAttacher.cpp 
)

# The document stuff
SET(Document_CPP_SRCS
    ../App/Annotation.cpp
    ../App/Document.cpp
    ../App/DocumentObject.cpp
    ../App/Extension.cpp
    ../App/ExtensionPyImp.cpp
    ../App/DocumentObjectExtension.cpp
    ../App/DocumentObjectExtensionPyImp.cpp
    ../App/ExtensionContainer.cpp
    ../App/ExtensionContainerPyImp.cpp
    ../App/GroupExtension.cpp
    ../App/GroupExtensionPyImp.cpp
    ../App/DocumentObjectFileIncluded.cpp
    ../App/DocumentObjectGroup.cpp
    ../App/DocumentObjectGroupPyImp.cpp
    ../App/GeoFeaturePyImp.cpp
    ../App/DocumentObjectPyImp.cpp
    ../App/DocumentObserver.cpp
    ../App/DocumentObserverPython.cpp
    ../App/DocumentPyImp.cpp
    ../App/Expression.cpp
    ../App/FeaturePython.cpp
    ../App/FeatureTest.cpp
    ../App/GeoFeature.cpp
    ../App/GeoFeatureGroupExtensionPyImp.cpp
    ../App/GeoFeatureGroupExtension.cpp
    ../App/OriginGroupExtensionPyImp.cpp
    ../App/OriginGroupExtension.cpp
    ../App/PartPyImp.cpp
    ../App/Part.cpp
    ../App/Origin.cpp
    ../App/Path.cpp
    ../App/InventorObject.cpp
    ../App/MeasureDistance.cpp
    ../App/AppPlacement.cpp
    ../App/OriginFeature.cpp
    ../App/Range.cpp
    ../App/Transactions.cpp
    ../App/TransactionalObject.cpp
    ../App/VRMLObject.cpp
    ../App/MaterialObject.cpp
    ../App/MergeDocuments.cpp
    ../App/TextDocument.cpp
)

SET(Document_HPP_SRCS
    ../App/Annotation.h
    ../App/Document.h
    ../App/DocumentObject.h
    ../App/Extension.h
    ../App/ExtensionContainer.h
    ../App/GroupExtension.h
    ../App/DocumentObjectExtension.h
    ../App/DocumentObjectFileIncluded.h
    ../App/DocumentObjectGroup.h
    ../App/DocumentObserver.h
    ../App/DocumentObserverPython.h
    ../App/Expression.h
    ../App/ExpressionVisitors.h
    ../App/FeatureCustom.h
    ../App/FeaturePython.h
    ../App/FeaturePythonPyImp.h
    ../App/FeaturePythonPyImp.inl
    ../App/FeatureTest.h
    ../App/GeoFeature.h
    ../App/GeoFeatureGroupExtension.h
    ../App/OriginGroupExtension.h
    ../App/Part.h
    ../App/Origin.h
    ../App/Path.h
    ../App/InventorObject.h
    ../App/MeasureDistance.h
    ../App/AppPlacement.h
    ../App/OriginFeature.h
    ../App/Range.h
    ../App/Transactions.h
    ../App/TransactionalObject.h
    ../App/VRMLObject.h
    ../App/MaterialObject.h
    ../App/MergeDocuments.h
    ../App/TextDocument.h
)
SET(Document_SRCS
    ${Document_CPP_SRCS}
    ${Document_HPP_SRCS}
)
SOURCE_GROUP("Document" FILES ${Document_SRCS})

# The property stuff
SET(Properties_CPP_SRCS
    ../App/DynamicProperty.cpp
    ../App/ObjectIdentifier.cpp
    ../App/Property.cpp
    ../App/PropertyContainer.cpp
    ../App/PropertyContainerPyImp.cpp
    ../App/PropertyFile.cpp
    ../App/PropertyGeo.cpp
    ../App/PropertyLinks.cpp
    ../App/PropertyPythonObject.cpp
    ../App/PropertyStandard.cpp
    ../App/PropertyUnits.cpp
    ../App/PropertyExpressionEngine.cpp
)
SET(Properties_HPP_SRCS
    ../App/DynamicProperty.h
    ../App/ObjectIdentifier.h
    ../App/Property.h
    ../App/PropertyContainer.h
    ../App/PropertyFile.h
    ../App/PropertyGeo.h
    ../App/PropertyLinks.h
    ../App/PropertyPythonObject.h
    ../App/PropertyStandard.h
    ../App/PropertyUnits.h
    ../App/PropertyExpressionEngine.h
)
SET(Properties_SRCS
    ${Properties_CPP_SRCS}
    ${Properties_HPP_SRCS}
)
SOURCE_GROUP("Properties" FILES ${Properties_SRCS})

SET(FreeCADApp_CPP_SRCS
    ${Document_CPP_SRCS}
    ${Properties_CPP_SRCS}
    ../App/Application.cpp
    ../App/ApplicationPy.cpp
    ../App/Branding.cpp
    ../App/ColorModel.cpp
    ../App/ComplexGeoData.cpp
    ../App/ComplexGeoDataPyImp.cpp
    ../App/Enumeration.cpp
    ../App/Material.cpp
    ../App/MaterialPyImp.cpp
)

SET(FreeCADApp_HPP_SRCS
    ${Document_HPP_SRCS}
    ${Properties_HPP_SRCS}
    ../App/Application.h
    ../App/Branding.h
    ../App/ColorModel.h
    ../App/ComplexGeoData.h
    ../App/Enumeration.h
    ../App/Material.h
)

SET(FreeCADApp_SRCS
    ${FreeCADApp_CPP_SRCS}
    ${FreeCADApp_HPP_SRCS}
    ${FreeCADApp_XML_SRCS}
    ../App/FreeCADInit.py
    ../App/FreeCADTest.py
    ../App/PreCompiled.cpp
    ../App/PreCompiled.h
)
set(Gui_MOC_HDRS
    ../Gui/Action.h
    ../Gui/ActionFunction.h
    ../Gui/Assistant.h
    ../Gui/AutoSaver.h
    ../Gui/CallTips.h
    ../Gui/CombiView.h
    ../Gui/Control.h
    ../Gui/Clipping.h
    ../Gui/DemoMode.h
    ../Gui/DownloadItem.h
    ../Gui/DownloadManager.h
    ../Gui/DlgActionsImp.h
    ../Gui/DlgActivateWindowImp.h
    ../Gui/DlgUnitsCalculatorImp.h
    ../Gui/DlgCommandsImp.h
    ../Gui/DlgCustomizeImp.h
    ../Gui/DlgCustomizeSpaceball.h
    ../Gui/DlgCustomizeSpNavSettings.h
    ../Gui/DlgDisplayPropertiesImp.h
    ../Gui/DlgEditorImp.h
    ../Gui/DlgGeneralImp.h
    ../Gui/DlgInputDialogImp.h
    ../Gui/DlgKeyboardImp.h
    ../Gui/DlgMacroExecuteImp.h
    ../Gui/DlgMacroRecordImp.h
    ../Gui/DlgRunExternal.h
    ../Gui/DlgEditFileIncludeProptertyExternal.h
    ../Gui/DlgMaterialPropertiesImp.h
    ../Gui/DlgOnlineHelpImp.h
    ../Gui/DlgParameterImp.h
    ../Gui/DlgPreferencesImp.h
    ../Gui/DlgProjectInformationImp.h
    ../Gui/DlgProjectUtility.h
    ../Gui/DlgPropertyLink.h
    ../Gui/DlgReportViewImp.h
    ../Gui/DlgSettings3DViewImp.h
    ../Gui/DlgSettingsViewColor.h
    ../Gui/DlgSettingsColorGradientImp.h
    ../Gui/DlgSettingsDocumentImp.h
    ../Gui/DlgSettingsImageImp.h
    ../Gui/DlgSettingsMacroImp.h
    ../Gui/DlgSettingsUnitsImp.h
    ../Gui/DlgCheckableMessageBox.h
    ../Gui/DlgToolbarsImp.h
    ../Gui/DlgWorkbenchesImp.h
    ../Gui/TaskDlgRelocation.h
    ../Gui/TaskCSysDragger.h
    ../Gui/DlgUndoRedo.h
    ../Gui/DockWindow.h
    ../Gui/DockWindowManager.h
    ../Gui/DocumentRecovery.h
    ../Gui/EditorView.h
    ../Gui/TextDocumentEditorView.h
    ../Gui/ExpressionCompleter.h
    ../Gui/DlgExpressionInput.h
    ../Gui/FileDialog.h
    ../Gui/Flag.h
    ../Gui/GraphicsViewZoom.h
    ../Gui/GraphvizView.h
    ../Gui/GuiApplication.h
    ../Gui/GuiApplicationNativeEventAware.h
    ../Gui/InputVector.h
    ../Gui/MainWindow.h
    ../Gui/ManualAlignment.h
    ../Gui/MDIView.h
    ../Gui/NetworkRetriever.h
    ../Gui/OnlineDocumentation.h
    ../Gui/GuiPlacement.h
    ../Gui/InputField.h
    ../Gui/PrefWidgets.h
    ../Gui/ProgressBar.h
    ../Gui/PropertyPage.h
    ../Gui/PropertyView.h
    ../Gui/PythonConsole.h
    ../Gui/PythonDebugger.h
    ../Gui/PythonEditor.h
    ../Gui/QuantitySpinBox.h
    ../Gui/QListWidgetCustom.h
    ../Gui/QuantitySpinBox_p.h
    ../Gui/ReportView.h
    ../Gui/SceneInspector.h
    ../Gui/SelectionView.h
    ../Gui/SpinBox.h
    ../Gui/Splashscreen.h
    ../Gui/TextEdit.h
    ../Gui/TextureMapping.h
    ../Gui/ToolBox.h
    ../Gui/Transform.h
    ../Gui/Tree.h
    ../Gui/TreeView.h
    ../Gui/ProjectView.h
    ../Gui/View3DInventor.h
    ../Gui/WidgetFactory.h
    ../Gui/Widgets.h
    ../Gui/Language/Translator.h
    ../Gui/propertyeditor/PropertyEditor.h
    ../Gui/propertyeditor/PropertyItem.h
    ../Gui/propertyeditor/PropertyItemDelegate.h
    ../Gui/propertyeditor/PropertyModel.h
    ../Gui/TaskView/TaskAppearance.h
    ../Gui/TaskView/TaskSelectLinkProperty.h
    ../Gui/TaskView/TaskDialog.h
    ../Gui/TaskView/TaskWatcher.h
    ../Gui/TaskView/TaskEditControl.h
    ../Gui/TaskView/TaskView.h
    ../Gui/DAGView/DAGView.h
    ../Gui/DAGView/DAGModel.h
)

fc_wrap_cpp(Gui_MOC_SRCS ${Gui_MOC_HDRS})
SOURCE_GROUP("Moc" FILES ${Gui_MOC_SRCS})

if (BUILD_QT5)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp
        COMMAND ${Qt5Core_MOC_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.cpp)

    set_property(SOURCE GraphvizView.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp)
else()
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp
        COMMAND ${QT_MOC_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../Gui/GraphvizView.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../Gui/GraphvizView.cpp)

    set_property(SOURCE ../Gui/GraphvizView.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp)
endif()

SET(Gui_UIC_SRCS
    ../Gui/AboutApplication.ui
    ../Gui/Clipping.ui
    ../Gui/DemoMode.ui
    ../Gui/DlgActions.ui
    ../Gui/DlgActivateWindow.ui
    ../Gui/DlgUnitsCalculator.ui
    ../Gui/DlgAuthorization.ui
    ../Gui/DlgChooseIcon.ui
    ../Gui/DlgCommands.ui
    ../Gui/DlgCustomizeSpNavSettings.ui
    ../Gui/DlgDisplayProperties.ui
    ../Gui/DlgEditor.ui
    ../Gui/DlgGeneral.ui
    ../Gui/DlgInputDialog.ui
    ../Gui/DlgKeyboard.ui
    ../Gui/DlgMacroExecute.ui
    ../Gui/DlgRunExternal.ui
    ../Gui/DlgMacroRecord.ui
    ../Gui/DlgMaterialProperties.ui
    ../Gui/DlgOnlineHelp.ui
    ../Gui/DlgParameter.ui
    ../Gui/DlgPreferences.ui
    ../Gui/DlgProjectInformation.ui
    ../Gui/DlgProjectUtility.ui
    ../Gui/DlgPropertyLink.ui
    ../Gui/DlgReportView.ui
    ../Gui/DlgSettings3DView.ui
    ../Gui/DlgSettingsUnits.ui
    ../Gui/DlgSettingsViewColor.ui
    ../Gui/DlgSettingsColorGradient.ui
    ../Gui/DlgSettingsDocument.ui
    ../Gui/DlgSettingsImage.ui
    ../Gui/DlgSettingsMacro.ui
    ../Gui/DlgCheckableMessageBox.ui
    ../Gui/DlgToolbars.ui
    ../Gui/DlgWorkbenches.ui
    ../Gui/DlgTreeWidget.ui
    ../Gui/DlgLocationAngle.ui
    ../Gui/DlgLocationPos.ui
    ../Gui/DocumentRecovery.ui
    ../Gui/DownloadManager.ui
    ../Gui/DownloadItem.ui
    ../Gui/DlgExpressionInput.ui
    ../Gui/MouseButtons.ui
    ../Gui/SceneInspector.ui
    ../Gui/InputVector.ui
    ../Gui/GuiPlacement.ui
    ../Gui/TextureMapping.ui
    ../Gui/TaskView/TaskAppearance.ui
    ../Gui/TaskView/TaskSelectLinkProperty.ui
)

SET(Gui_RES_SRCS
    ../Gui/Icons/resource.qrc
    ../Gui/Language/translation.qrc
    ../Gui/iisTaskPanel/src/iisTaskPanel.qrc
    ../Gui/QSint/actionpanel/schemes.qrc
)

if (BUILD_QT5)
    QT5_ADD_RESOURCES(Gui_QRC_SRCS ${Gui_RES_SRCS})
    QT5_WRAP_UI(Gui_UIC_HDRS ${Gui_UIC_SRCS})
else()
    QT4_ADD_RESOURCES(Gui_QRC_SRCS ${Gui_RES_SRCS})
    QT4_WRAP_UI(Gui_UIC_HDRS ${Gui_UIC_SRCS})
endif()
SOURCE_GROUP("Uic" FILES ${Gui_UIC_HDRS})

# The command sources
SET(Command_CPP_SRCS
    ../Gui/Action.cpp
    ../Gui/ActionFunction.cpp
    ../Gui/Command.cpp
    ../Gui/CommandDoc.cpp
    ../Gui/CommandFeat.cpp
    ../Gui/CommandMacro.cpp
    ../Gui/CommandStd.cpp
    ../Gui/CommandWindow.cpp
    ../Gui/CommandTest.cpp
    ../Gui/CommandView.cpp
)
SET(Command_SRCS
    ${Command_CPP_SRCS}
    ../Gui/Action.h
    ../Gui/ActionFunction.h
    ../Gui/Command.h
)
SOURCE_GROUP("Command" FILES ${Command_SRCS})

# The dialog sources
SET(Dialog_CPP_SRCS
    ../Gui/Clipping.cpp
    ../Gui/DemoMode.cpp
    ../Gui/DlgActivateWindowImp.cpp
    ../Gui/DlgUnitsCalculatorImp.cpp
    ../Gui/DlgDisplayPropertiesImp.cpp
    ../Gui/DlgInputDialogImp.cpp
    ../Gui/DlgMacroExecuteImp.cpp
    ../Gui/DlgRunExternal.cpp
    ../Gui/DlgEditFileIncludeProptertyExternal.cpp
    ../Gui/DlgMacroRecordImp.cpp
    ../Gui/DlgMaterialPropertiesImp.cpp
    ../Gui/DlgParameterImp.cpp
    ../Gui/DlgProjectInformationImp.cpp
    ../Gui/DlgProjectUtility.cpp
    ../Gui/DlgPropertyLink.cpp
    ../Gui/DlgExpressionInput.cpp
    ../Gui/TaskDlgRelocation.cpp
    ../Gui/DlgCheckableMessageBox.cpp
    ../Gui/TaskCSysDragger.cpp
    ../Gui/DlgUndoRedo.cpp
    ../Gui/InputVector.cpp
    ../Gui/GuiPlacement.cpp
    ../Gui/PropertyPage.cpp
    ../Gui/SceneInspector.cpp
    ../Gui/TextureMapping.cpp
    ../Gui/Transform.cpp
    ../Gui/DownloadItem.cpp
    ../Gui/DownloadManager.cpp
    ../Gui/DocumentRecovery.cpp
)

SET(Dialog_HPP_SRCS
    ../Gui/Clipping.h
    ../Gui/DemoMode.h
    ../Gui/DlgActivateWindowImp.h
    ../Gui/DlgUnitsCalculatorImp.h
    ../Gui/DlgDisplayPropertiesImp.h
    ../Gui/DlgInputDialogImp.h
    ../Gui/DlgMacroExecuteImp.h
    ../Gui/DlgRunExternal.h
    ../Gui/DlgEditFileIncludeProptertyExternal.h
    ../Gui/DlgMacroRecordImp.h
    ../Gui/DlgMaterialPropertiesImp.h
    ../Gui/DlgParameterImp.h
    ../Gui/DlgProjectInformationImp.h
    ../Gui/DlgProjectUtility.h
    ../Gui/DlgPropertyLink.h
    ../Gui/DlgCheckableMessageBox.h
    ../Gui/DlgExpressionInput.h
    ../Gui/TaskDlgRelocation.h
    ../Gui/TaskCSysDragger.h
    ../Gui/DlgUndoRedo.h
    ../Gui/InputVector.h
    ../Gui/GuiPlacement.h
    ../Gui/PropertyPage.h
    ../Gui/SceneInspector.h
    ../Gui/TextureMapping.h
    ../Gui/Transform.h
    ../Gui/DownloadItem.h
    ../Gui/DownloadManager.h
    ../Gui/DocumentRecovery.h
)

SET(Dialog_SRCS
    ${Dialog_CPP_SRCS}
    ${Dialog_HPP_SRCS}
    ../Gui/AboutApplication.ui
    ../Gui/Clipping.ui
    ../Gui/DemoMode.ui
    ../Gui/DlgActivateWindow.ui
    ../Gui/DlgUnitsCalculator.ui
    ../Gui/DlgAuthorization.ui
    ../Gui/DlgDisplayProperties.ui
    ../Gui/DlgInputDialog.ui
    ../Gui/DlgLocationAngle.ui
    ../Gui/DlgLocationPos.ui
    ../Gui/DlgMacroExecute.ui
    ../Gui/DlgRunExternal.ui
    ../Gui/DlgMacroRecord.ui
    ../Gui/DlgMaterialProperties.ui
    ../Gui/DlgParameter.ui
    ../Gui/DlgProjectInformation.ui
    ../Gui/DlgProjectUtility.ui
    ../Gui/DlgPropertyLink.ui
    ../Gui/DlgCheckableMessageBox.ui
    ../Gui/DlgTreeWidget.ui
    ../Gui/DlgExpressionInput.ui
    ../Gui/DownloadManager.ui
    ../Gui/DownloadItem.ui
    ../Gui/DocumentRecovery.ui
    ../Gui/MouseButtons.ui
    ../Gui/InputVector.ui
    ../Gui/GuiPlacement.ui
    ../Gui/SceneInspector.ui
    ../Gui/TextureMapping.ui
)
SOURCE_GROUP("Dialog" FILES ${Dialog_SRCS})

# The customize dialog sources
SET(Dialog_Customize_CPP_SRCS
    ../Gui/DlgActionsImp.cpp
    ../Gui/DlgCommandsImp.cpp
    ../Gui/DlgCustomizeImp.cpp
    ../Gui/DlgCustomizeSpaceball.cpp
    ../Gui/DlgCustomizeSpNavSettings.cpp
    ../Gui/DlgKeyboardImp.cpp
    ../Gui/DlgToolbarsImp.cpp
    ../Gui/DlgWorkbenchesImp.cpp
    ../Gui/QListWidgetCustom.cpp
)
SET(Dialog_Customize_HPP_SRCS
    ../Gui/DlgActionsImp.h
    ../Gui/DlgCommandsImp.h
    ../Gui/DlgCustomizeImp.h
    ../Gui/DlgCustomizeSpaceball.h
    ../Gui/DlgCustomizeSpNavSettings.h
    ../Gui/DlgKeyboardImp.h
    ../Gui/DlgToolbarsImp.h
    ../Gui/DlgWorkbenchesImp.h
    ../Gui/QListWidgetCustom.h
)
SET(Dialog_Customize_SRCS
    ${Dialog_Customize_CPP_SRCS}
    ${Dialog_Customize_HPP_SRCS}
    ../Gui/DlgActions.ui
    ../Gui/DlgChooseIcon.ui
    ../Gui/DlgCommands.ui
    ../Gui/DlgCustomizeSpNavSettings.ui
    ../Gui/DlgKeyboard.ui
    ../Gui/DlgToolbars.ui
    ../Gui/DlgWorkbenches.ui
)
SOURCE_GROUP("Dialog\\Customize" FILES ${Dialog_Customize_SRCS})

# The settings dialog sources
SET(Dialog_Settings_CPP_SRCS
    ../Gui/DlgEditorImp.cpp
    ../Gui/DlgGeneralImp.cpp
    ../Gui/DlgOnlineHelpImp.cpp
    ../Gui/DlgPreferencesImp.cpp
    ../Gui/DlgReportViewImp.cpp
    ../Gui/DlgSettings3DViewImp.cpp
    ../Gui/DlgSettingsUnitsImp.cpp
    ../Gui/DlgSettingsViewColor.cpp
    ../Gui/DlgSettingsColorGradientImp.cpp
    ../Gui/DlgSettingsDocumentImp.cpp
    ../Gui/DlgSettingsImageImp.cpp
    ../Gui/DlgSettingsMacroImp.cpp
)
SET(Dialog_Settings_HPP_SRCS
    ../Gui/DlgEditorImp.h
    ../Gui/DlgGeneralImp.h
    ../Gui/DlgOnlineHelpImp.h
    ../Gui/DlgPreferencesImp.h
    ../Gui/DlgReportViewImp.h
    ../Gui/DlgSettings3DViewImp.h
    ../Gui/DlgSettingsUnitsImp.h
    ../Gui/DlgSettingsViewColor.h
    ../Gui/DlgSettingsColorGradientImp.h
    ../Gui/DlgSettingsDocumentImp.h
    ../Gui/DlgSettingsImageImp.h
    ../Gui/DlgSettingsMacroImp.h
)
SET(Dialog_Settings_SRCS
    ${Dialog_Settings_CPP_SRCS}
    ${Dialog_Settings_HPP_SRCS}
    ../Gui/DlgEditor.ui
    ../Gui/DlgGeneral.ui
    ../Gui/DlgOnlineHelp.ui
    ../Gui/DlgPreferences.ui
    ../Gui/DlgReportView.ui
    ../Gui/DlgSettings3DView.ui
    ../Gui/DlgSettingsUnits.ui
    ../Gui/DlgSettingsViewColor.ui
    ../Gui/DlgSettingsColorGradient.ui
    ../Gui/DlgSettingsDocument.ui
    ../Gui/DlgSettingsImage.ui
    ../Gui/DlgSettingsMacro.ui
)
SOURCE_GROUP("Dialog\\Settings" FILES ${Dialog_Settings_SRCS})

# The dock windows sources
SET(Dock_Windows_CPP_SRCS
    ../Gui/CombiView.cpp
    ../Gui/DockWindow.cpp
    ../Gui/PropertyView.cpp
    ../Gui/ReportView.cpp
    ../Gui/SelectionView.cpp
    ../Gui/ToolBox.cpp
    ../Gui/Tree.cpp
    ../Gui/TreeView.cpp
    ../Gui/ProjectView.cpp
    ../Gui/DAGView/DAGView.cpp
    ../Gui/DAGView/DAGModel.cpp
    ../Gui/DAGView/DAGRectItem.cpp
    ../Gui/DAGView/DAGModelGraph.cpp
    ../Gui/DAGView/DAGFilter.cpp
)
SET(Dock_Windows_HPP_SRCS
    ../Gui/CombiView.h
    ../Gui/DockWindow.h
    ../Gui/PropertyView.h
    ../Gui/ReportView.h
    ../Gui/SelectionView.h
    ../Gui/ToolBox.h
    ../Gui/Tree.h
    ../Gui/TreeView.h
    ../Gui/ProjectView.h
    ../Gui/DAGView/DAGView.h
    ../Gui/DAGView/DAGModel.h
    ../Gui/DAGView/DAGRectItem.h
    ../Gui/DAGView/DAGModelGraph.h
    ../Gui/DAGView/DAGFilter.h
)
SET(Dock_Windows_SRCS
    ${Dock_Windows_CPP_SRCS}
    ${Dock_Windows_HPP_SRCS}
)
SOURCE_GROUP("Dock Windows" FILES ${Dock_Windows_SRCS})

# The editor sources
SET(Editor_CPP_SRCS
    ../Gui/CallTips.cpp
    ../Gui/EditorView.cpp
    ../Gui/TextDocumentEditorView.cpp
    ../Gui/PythonConsole.cpp
    ../Gui/PythonConsolePy.cpp
    ../Gui/PythonDebugger.cpp
    ../Gui/PythonEditor.cpp
    ../Gui/SyntaxHighlighter.cpp
    ../Gui/TextEdit.cpp
)
SET(Editor_HPP_SRCS
    ../Gui/CallTips.h
    ../Gui/EditorView.h
    ../Gui/TextDocumentEditorView.h
    ../Gui/PythonConsole.h
    ../Gui/PythonConsolePy.h
    ../Gui/PythonDebugger.h
    ../Gui/PythonEditor.h
    ../Gui/SyntaxHighlighter.h
    ../Gui/TextEdit.h
)
SET(Editor_SRCS
    ${Editor_CPP_SRCS}
    ${Editor_HPP_SRCS}
)
SOURCE_GROUP("Editor" FILES ${Editor_SRCS})

# The help system
SET(Help_CPP_SRCS
    ../Gui/Assistant.cpp
    ../Gui/NetworkRetriever.cpp
    ../Gui/OnlineDocumentation.cpp
    ../Gui/WhatsThis.cpp
)
SET(Help_SRCS
    ${Help_CPP_SRCS}
    ../Gui/Assistant.h
    ../Gui/NetworkRetriever.h
    ../Gui/OnlineDocumentation.h
    ../Gui/WhatsThis.h
)
SOURCE_GROUP("Help" FILES ${Help_SRCS})


# The i18n sources
SET(Language_SRCS
    ../Gui/Language/Translator.cpp
    ../Gui/Language/Translator.h
)
SOURCE_GROUP("Language" FILES ${Language_SRCS})

# The property editor
SET(Propertyeditor_SRCS
    ../Gui/propertyeditor/PropertyEditor.cpp
    ../Gui/propertyeditor/PropertyEditor.h
    ../Gui/propertyeditor/PropertyItem.cpp
    ../Gui/propertyeditor/PropertyItem.h
    ../Gui/propertyeditor/PropertyItemDelegate.cpp
    ../Gui/propertyeditor/PropertyItemDelegate.h
    ../Gui/propertyeditor/PropertyModel.cpp
    ../Gui/propertyeditor/PropertyModel.h
)
SOURCE_GROUP("Propertyeditor" FILES ${Propertyeditor_SRCS})

# The task view
SET(Task_View_SRCS
    ../Gui/TaskView/TaskAppearance.cpp
    ../Gui/TaskView/TaskAppearance.h
    ../Gui/TaskView/TaskAppearance.ui
    ../Gui/TaskView/TaskSelectLinkProperty.cpp
    ../Gui/TaskView/TaskSelectLinkProperty.h
    ../Gui/TaskView/TaskSelectLinkProperty.ui
    ../Gui/TaskView/TaskEditControl.cpp
    ../Gui/TaskView/TaskEditControl.h
    ../Gui/TaskView/TaskEditControl.ui
    ../Gui/TaskView/TaskView.cpp
    ../Gui/TaskView/TaskView.h
    ../Gui/TaskView/TaskDialog.cpp
    ../Gui/TaskView/TaskDialog.h
    ../Gui/TaskView/TaskDialogPython.cpp
    ../Gui/TaskView/TaskDialogPython.h
    ../Gui/TaskView/TaskWatcher.cpp
    ../Gui/TaskView/TaskWatcher.h
)
SOURCE_GROUP("Task View" FILES ${Task_View_SRCS})

SET(iisTaskPanel_SRCS
    ../Gui/iisTaskPanel/src/iisfreecadscheme.cpp
    ../Gui/iisTaskPanel/src/iisfreecadscheme.h
    ../Gui/iisTaskPanel/src/iisiconlabel.cpp
    ../Gui/iisTaskPanel/src/iisiconlabel.h
    ../Gui/iisTaskPanel/src/iistaskbox.cpp
    ../Gui/iisTaskPanel/src/iistaskbox.h
    ../Gui/iisTaskPanel/src/iistaskgroup.cpp
    ../Gui/iisTaskPanel/src/iistaskgroup.h
    ../Gui/iisTaskPanel/src/iistaskheader.cpp
    ../Gui/iisTaskPanel/src/iistaskheader.h
    ../Gui/iisTaskPanel/src/iistaskpanel.cpp
    ../Gui/iisTaskPanel/src/iistaskpanel.h
    ../Gui/iisTaskPanel/src/iistaskpanelscheme.cpp
    ../Gui/iisTaskPanel/src/iistaskpanelscheme.h
    ../Gui/iisTaskPanel/src/iiswinxptaskpanelscheme.cpp
    ../Gui/iisTaskPanel/src/iiswinxptaskpanelscheme.h
    ../Gui/iisTaskPanel/src/iistaskpanel_global.h
)
SOURCE_GROUP("Widget\\iisTaskPanel" FILES ${iisTaskPanel_SRCS})
set(iis_MOC_HDRS
    ../Gui/iisTaskPanel/src/iisiconlabel.h
    ../Gui/iisTaskPanel/src/iistaskbox.h
    ../Gui/iisTaskPanel/src/iistaskgroup.h
    ../Gui/iisTaskPanel/src/iistaskheader.h
)

if (BUILD_QT5)
    qt5_wrap_cpp(iis_MOC_SRCS ${iis_MOC_HDRS})
else()
    qt4_wrap_cpp(iis_MOC_SRCS ${iis_MOC_HDRS})
endif()
SOURCE_GROUP("Widget\\iisTaskPanel\\Mocs" FILES ${iis_MOC_SRCS})

SET(qsintActionPanel_SRCS
    ../Gui/QSint/actionpanel/actionbox.cpp
    ../Gui/QSint/actionpanel/actionbox.h
    ../Gui/QSint/actionpanel/actionlabel.cpp
    ../Gui/QSint/actionpanel/actionlabel.h
    ../Gui/QSint/actionpanel/actiongroup.cpp
    ../Gui/QSint/actionpanel/actiongroup.h
    ../Gui/QSint/actionpanel/actionpanel.cpp
    ../Gui/QSint/actionpanel/actionpanel.h
    ../Gui/QSint/actionpanel/actionpanelscheme.cpp
    ../Gui/QSint/actionpanel/actionpanelscheme.h
    ../Gui/QSint/actionpanel/androidpanelscheme.cpp
    ../Gui/QSint/actionpanel/androidpanelscheme.h
    ../Gui/QSint/actionpanel/macpanelscheme.cpp
    ../Gui/QSint/actionpanel/macpanelscheme.h
    ../Gui/QSint/actionpanel/taskgroup_p.cpp
    ../Gui/QSint/actionpanel/taskgroup_p.h
    ../Gui/QSint/actionpanel/taskheader_p.cpp
    ../Gui/QSint/actionpanel/taskheader_p.h
    ../Gui/QSint/actionpanel/winvistapanelscheme.cpp
    ../Gui/QSint/actionpanel/winvistapanelscheme.h
    ../Gui/QSint/actionpanel/winxppanelscheme.cpp
    ../Gui/QSint/actionpanel/winxppanelscheme.h
    ../Gui/QSint/actionpanel/freecadscheme.cpp
    ../Gui/QSint/actionpanel/freecadscheme.h
)
SOURCE_GROUP("Widget\\QSintActionPanel" FILES ${qsintActionPanel_SRCS})
set(qsint_MOC_HDRS
    ../Gui/QSint/actionpanel/actionbox.h
    ../Gui/QSint/actionpanel/actionlabel.h
    ../Gui/QSint/actionpanel/actiongroup.h
    ../Gui/QSint/actionpanel/actionpanel.h
    ../Gui/QSint/actionpanel/taskheader_p.h
)
if(BUILD_QT5)
    qt5_wrap_cpp(qsint_MOC_SRCS ${qsint_MOC_HDRS})
else()
    qt4_wrap_cpp(qsint_MOC_SRCS ${qsint_MOC_HDRS})
endif()
SOURCE_GROUP("Widget\\QSintActionPanel\\Mocs" FILES ${qsint_MOC_SRCS})

#SET(Resource_RCS
#   Icons/resource.qrc
#   Language/translation.qrc)
#qt4_add_resources(Resource_SRCS ${Resource_RCS})
#SET(Resource_SRCS
#   ${Resource_SRCS}
#   Icons/resource.qrc
#   Language/translation.qrc
#)
#SOURCE_GROUP("Resource" FILES ${Resource_SRCS})

# The 3d view
SET(View3D_CPP_SRCS
    ../Gui/Flag.cpp
    ../Gui/GLPainter.cpp
    ../Gui/MouseSelection.cpp
    ../Gui/NavigationStyle.cpp
    ../Gui/InventorNavigationStyle.cpp
    ../Gui/CADNavigationStyle.cpp
    ../Gui/BlenderNavigationStyle.cpp
    ../Gui/MayaGestureNavigationStyle.cpp
    ../Gui/OpenCascadeNavigationStyle.cpp
    ../Gui/TouchpadNavigationStyle.cpp
    ../Gui/GestureNavigationStyle.cpp
    ../Gui/SplitView3DInventor.cpp
    ../Gui/View.cpp
    ../Gui/View3DInventor.cpp
    ../Gui/View3DInventorExamples.cpp
    ../Gui/View3DInventorViewer.cpp
    ../Gui/View3DInventorRiftViewer.cpp
    ../Gui/CoinRiftWidget.cpp
    ../Gui/View3DPy.cpp
    ../Gui/View3DViewerPy.cpp
)
SET(View3D_SRCS
    ${View3D_CPP_SRCS}
    ../Gui/Flag.h
    ../Gui/GLPainter.h
    ../Gui/MouseSelection.h
    ../Gui/NavigationStyle.h
    ../Gui/SplitView3DInventor.h
    ../Gui/View.h
    ../Gui/View3DInventor.h
    ../Gui/View3DInventorExamples.h
    ../Gui/View3DInventorViewer.h
    ../Gui/View3DPy.h
    ../Gui/View3DInventorRiftViewer.h
    ../Gui/CoinRiftWidget.h
    ../Gui/View3DViewerPy.h
)
SOURCE_GROUP("View3D" FILES ${View3D_SRCS})

#quarter sources
FILE(GLOB_RECURSE Quarter_CPP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../Gui/Quarter/*.cpp)
FILE(GLOB_RECURSE Quarter_H_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../Gui/Quarter/*.h)

SET(Quarter_MOC_HDR
    ../Gui/Quarter/SignalThread.h
    ../Gui/Quarter/InteractionMode.h
    ../Gui/Quarter/SensorManager.h
    ../Gui/Quarter/ContextMenu.h
    ../Gui/Quarter/eventhandlers/FocusHandler.h
    ../Gui/Quarter/eventhandlers/DragDropHandler.h
    ../Gui/Quarter/eventhandlers/EventFilter.h
    ../Gui/Quarter/QuarterWidget.h
)
if(BUILD_QT5)
    qt5_wrap_cpp(Quarter_MOC_SRCS ${Quarter_MOC_HDR})
else()
    qt4_wrap_cpp(Quarter_MOC_SRCS ${Quarter_MOC_HDR})
endif()

SET(Quarter_SRCS 
    ${Quarter_CPP_SRC} 
    ${Quarter_H_SRC}
    ${Quarter_MOC_SRCS}
)
SOURCE_GROUP("Quarter" FILES ${Quarter_SRCS})

# The view provider sources
SET(Viewprovider_CPP_SRCS
    ../Gui/ViewProvider.cpp
    ../Gui/ViewProviderExtension.cpp
    ../Gui/ViewProviderGroupExtension.cpp
    ../Gui/ViewProviderGeoFeatureGroupExtension.cpp
    ../Gui/ViewProviderOriginGroupExtension.cpp
    ../Gui/ViewProviderAnnotation.cpp
    ../Gui/ViewProviderDocumentObject.cpp
    ../Gui/ViewProviderDocumentObjectGroup.cpp
    ../Gui/ViewProviderDocumentObjectPyImp.cpp
    ../Gui/ViewProviderExtern.cpp
    ../Gui/ViewProviderFeature.cpp
    ../Gui/ViewProviderGeometryObject.cpp
    ../Gui/ViewProviderInventorObject.cpp
    ../Gui/ViewProviderMeasureDistance.cpp
    ../Gui/ViewProviderPyImp.cpp
    ../Gui/ViewProviderPythonFeature.cpp
    ../Gui/ViewProviderVRMLObject.cpp
    ../Gui/ViewProviderBuilder.cpp
    ../Gui/ViewProviderPlacement.cpp
    ../Gui/ViewProviderOriginFeature.cpp
    ../Gui/ViewProviderPlane.cpp
    ../Gui/ViewProviderLine.cpp
    ../Gui/ViewProviderGeoFeatureGroup.cpp
    ../Gui/ViewProviderOriginGroup.cpp
    ../Gui/ViewProviderPart.cpp
    ../Gui/ViewProviderOrigin.cpp
    ../Gui/ViewProviderMaterialObject.cpp
    ../Gui/ViewProviderTextDocument.cpp
)
SET(Viewprovider_SRCS
    ${Viewprovider_CPP_SRCS}
    ../Gui/ViewProvider.h
    ../Gui/ViewProviderExtension.h
    ../Gui/ViewProviderGroupExtension.h
    ../Gui/ViewProviderGeoFeatureGroupExtension.h
    ../Gui/ViewProviderOriginGroupExtension.h
    ../Gui/ViewProviderAnnotation.h
    ../Gui/ViewProviderDocumentObject.h
    ../Gui/ViewProviderDocumentObjectGroup.h
    ../Gui/ViewProviderExtern.h
    ../Gui/ViewProviderFeature.h
    ../Gui/ViewProviderGeometryObject.h
    ../Gui/ViewProviderInventorObject.h
    ../Gui/ViewProviderMeasureDistance.h
    ../Gui/ViewProviderPythonFeature.h
    ../Gui/ViewProviderVRMLObject.h
    ../Gui/ViewProviderBuilder.h
    ../Gui/ViewProviderPlacement.h
    ../Gui/ViewProviderOriginFeature.h
    ../Gui/ViewProviderPlane.h
    ../Gui/ViewProviderLine.h
    ../Gui/ViewProviderGeoFeatureGroup.h
    ../Gui/ViewProviderOriginGroup.h
    ../Gui/ViewProviderPart.h
    ../Gui/ViewProviderOrigin.h
    ../Gui/ViewProviderMaterialObject.h
    ../Gui/ViewProviderTextDocument.h
)
SOURCE_GROUP("View3D\\Viewprovider" FILES ${Viewprovider_SRCS})

# The Inventor sources
SET(Inventor_CPP_SRCS
    ../Gui/Inventor/SoDrawingGrid.cpp
    ../Gui/Inventor/SoAutoZoomTranslation.cpp
    ../Gui/Inventor/MarkerBitmaps.cpp
    ../Gui/Inventor/SmSwitchboard.cpp
    ../Gui/SoFCBackgroundGradient.cpp
    ../Gui/SoFCBoundingBox.cpp
    ../Gui/SoFCColorBar.cpp
    ../Gui/SoFCColorGradient.cpp
    ../Gui/SoFCColorLegend.cpp
    ../Gui/SoFCDB.cpp
    ../Gui/SoFCInteractiveElement.cpp
    ../Gui/SoFCOffscreenRenderer.cpp
    ../Gui/SoFCSelection.cpp
    ../Gui/SoFCUnifiedSelection.cpp
    ../Gui/SoFCSelectionAction.cpp
    ../Gui/SoFCVectorizeSVGAction.cpp
    ../Gui/SoFCVectorizeU3DAction.cpp
    ../Gui/SoNavigationDragger.cpp
    ../Gui/SoAxisCrossKit.cpp
    ../Gui/SoTextLabel.cpp
    ../Gui/SoTouchEvents.cpp
    ../Gui/SoFCCSysDragger.cpp
)
SET(Inventor_SRCS
    ${Inventor_CPP_SRCS}
    ../Gui/Inventor/SoDrawingGrid.h
    ../Gui/Inventor/SoAutoZoomTranslation.h
    ../Gui/Inventor/MarkerBitmaps.h
    ../Gui/Inventor/SmSwitchboard.h
    ../Gui/SoFCBackgroundGradient.h
    ../Gui/SoFCBoundingBox.h
    ../Gui/SoFCColorBar.h
    ../Gui/SoFCColorGradient.h
    ../Gui/SoFCColorLegend.h
    ../Gui/SoFCDB.h
    ../Gui/SoFCInteractiveElement.h
    ../Gui/SoFCOffscreenRenderer.h
    ../Gui/SoFCSelection.h
    ../Gui/SoFCUnifiedSelection.h
    ../Gui/SoFCSelectionAction.h
    ../Gui/SoFCVectorizeSVGAction.h
    ../Gui/SoFCVectorizeU3DAction.h
    ../Gui/SoNavigationDragger.h
    ../Gui/SoAxisCrossKit.h
    ../Gui/SoTextLabel.h
    ../Gui/SoTouchEvents.h
    ../Gui/SoFCCSysDragger.h
)
SOURCE_GROUP("View3D\\Inventor" FILES ${Inventor_SRCS})

# The widget sources
SET(Widget_CPP_SRCS
    ../Gui/FileDialog.cpp
    ../Gui/MainWindow.cpp
    ../Gui/PrefWidgets.cpp
    ../Gui/InputField.cpp
    ../Gui/ProgressBar.cpp
    ../Gui/QuantitySpinBox.cpp
    ../Gui/SpinBox.cpp
    ../Gui/Splashscreen.cpp
    ../Gui/WidgetFactory.cpp
    ../Gui/Widgets.cpp
    ../Gui/Window.cpp
)
SET(Widget_HPP_SRCS
    ../Gui/FileDialog.h
    ../Gui/MainWindow.h
    ../Gui/PrefWidgets.h
    ../Gui/InputField.h
    ../Gui/ProgressBar.h
    ../Gui/QuantitySpinBox.h
    ../Gui/QuantitySpinBox_p.h
    ../Gui/SpinBox.h
    ../Gui/Splashscreen.h
    ../Gui/WidgetFactory.h
    ../Gui/Widgets.h
    ../Gui/Window.h
)
SET(Widget_SRCS
    ${Widget_CPP_SRCS}
    ${Widget_HPP_SRCS}
)
SOURCE_GROUP("Widget" FILES ${Widget_SRCS})

# The view sources
SET(View_CPP_SRCS
    ../Gui/MDIView.cpp
    ../Gui/GraphvizView.cpp
    ../Gui/ActiveObjectList.cpp
)
SET(View_HPP_SRCS
    ../Gui/MDIView.h
    ../Gui/GraphvizView.h
    ../Gui/ActiveObjectList.h
)
SET(View_SRCS
    ${View_CPP_SRCS}
    ${View_HPP_SRCS}
)
SOURCE_GROUP("View" FILES ${View_SRCS})

# The workbench sources
SET(Workbench_CPP_SRCS
    ../Gui/DockWindowManager.cpp
    ../Gui/MenuManager.cpp
    ../Gui/PythonWorkbenchPyImp.cpp
    ../Gui/ToolBarManager.cpp
    ../Gui/ToolBoxManager.cpp
    ../Gui/Workbench.cpp
    ../Gui/WorkbenchFactory.cpp
    ../Gui/WorkbenchManager.cpp
    ../Gui/WorkbenchPyImp.cpp
)
SET(Workbench_SRCS
    ${Workbench_CPP_SRCS}
    ../Gui/DockWindowManager.h
    ../Gui/MenuManager.h
    ../Gui/ToolBarManager.h
    ../Gui/ToolBoxManager.h
    ../Gui/Workbench.h
    ../Gui/WorkbenchFactory.h
    ../Gui/WorkbenchManager.h
)
SOURCE_GROUP("Workbench" FILES ${Workbench_SRCS})

SET(Selection_SRCS
    ../Gui/SelectionObjectPyImp.cpp
    ../Gui/SelectionObject.h
    ../Gui/SelectionObject.cpp
    ../Gui/Selection.h
    ../Gui/Selection.cpp
    ../Gui/SelectionFilter.h
    ../Gui/SelectionFilter.cpp
    ../Gui/SelectionFilter.y
    ../Gui/SelectionFilter.l
)
SOURCE_GROUP("Selection" FILES ${Selection_SRCS})

# The FreeCADGui sources
SET(FreeCADGui_CPP_SRCS
    ../Gui/Application.cpp
    ../Gui/ApplicationPy.cpp
    ../Gui/AutoSaver.cpp
    ../Gui/BitmapFactory.cpp
    ../Gui/Document.cpp
    ../Gui/DocumentModel.cpp
    ../Gui/DocumentPyImp.cpp
    ../Gui/DocumentObserver.cpp
    ../Gui/ExpressionBinding.cpp
    ../Gui/GraphicsViewZoom.cpp
    ../Gui/ExpressionCompleter.cpp
    ../Gui/GuiApplication.cpp
    ../Gui/GuiApplicationNativeEventAware.cpp
    ../Gui/GuiConsole.cpp
    ../Gui/Macro.cpp
    ../Gui/MergeDocuments.cpp
    ../Gui/resource.cpp
    ../Gui/Control.cpp
    ../Gui/SpaceballEvent.cpp
    ../Gui/Thumbnail.cpp
    ../Gui/Utilities.cpp
    ../Gui/WaitCursor.cpp
    ../Gui/ManualAlignment.cpp
    ../Gui/TransactionObject.cpp
    ../Gui/WinNativeGestureRecognizers.cpp
)
SET(FreeCADGui_SRCS 
    ../Gui/Application.h
    ../Gui/AutoSaver.h
    ../Gui/BitmapFactory.h
    ../Gui/Document.h
    ../Gui/DocumentModel.h
    ../Gui/DocumentObserver.h
    ../Gui/ExpressionBinding.cpp
    ../Gui/ExpressionCompleter.h
    ../Gui/FreeCADGuiInit.py
    ../Gui/GraphicsViewZoom.h
    ../Gui/GuiApplication.h
    ../Gui/GuiApplicationNativeEventAware.h
    ../Gui/GuiConsole.h
    ../Gui/InventorAll.h
    ../Gui/Macro.h
    ../Gui/MergeDocuments.h
    ../Gui/MetaTypes.h
    ../Gui/PreCompiled.cpp
    ../Gui/PreCompiled.h
    ../Gui/Qt4All.h
    ../Gui/Control.h
    ../Gui/SpaceballEvent.h
    ../Gui/Thumbnail.h
    ../Gui/Utilities.h
    ../Gui/WaitCursor.h
    ../Gui/ManualAlignment.h
    ../Gui/TransactionObject.h
    ../Gui/WinNativeGestureRecognizers.h
)

SET(Features_SRCS
    ../Mod/Part/App/FeaturePartBoolean.cpp
    ../Mod/Part/App/FeaturePartBoolean.h
    ../Mod/Part/App/FeaturePartBox.cpp
    ../Mod/Part/App/FeaturePartBox.h
    ../Mod/Part/App/FeaturePartCircle.cpp
    ../Mod/Part/App/FeaturePartCircle.h
    ../Mod/Part/App/FeaturePartCommon.cpp
    ../Mod/Part/App/FeaturePartCommon.h
    ../Mod/Part/App/FeaturePartCurveNet.cpp
    ../Mod/Part/App/FeaturePartCurveNet.h
    ../Mod/Part/App/FeaturePartCut.cpp
    ../Mod/Part/App/FeaturePartCut.h
    ../Mod/Part/App/FeaturePartFuse.cpp
    ../Mod/Part/App/FeaturePartFuse.h
    ../Mod/Part/App/FeaturePartImportBrep.cpp
    ../Mod/Part/App/FeaturePartImportBrep.h
    ../Mod/Part/App/FeaturePartImportIges.cpp
    ../Mod/Part/App/FeaturePartImportIges.h
    ../Mod/Part/App/FeaturePartImportStep.cpp
    ../Mod/Part/App/FeaturePartImportStep.h
    ../Mod/Part/App/FeaturePartPolygon.cpp
    ../Mod/Part/App/FeaturePartPolygon.h
    ../Mod/Part/App/FeaturePartSection.cpp
    ../Mod/Part/App/FeaturePartSection.h
    ../Mod/Part/App/FeaturePartSpline.cpp
    ../Mod/Part/App/FeaturePartSpline.h
    ../Mod/Part/App/FeatureChamfer.cpp
    ../Mod/Part/App/FeatureChamfer.h
    ../Mod/Part/App/FeatureCompound.cpp
    ../Mod/Part/App/FeatureCompound.h
    ../Mod/Part/App/FeatureExtrusion.cpp
    ../Mod/Part/App/FeatureExtrusion.h
    ../Mod/Part/App/FeatureFace.cpp
    ../Mod/Part/App/FeatureFace.h
    ../Mod/Part/App/FeatureFillet.cpp
    ../Mod/Part/App/FeatureFillet.h
    ../Mod/Part/App/FeatureMirroring.cpp
    ../Mod/Part/App/FeatureMirroring.h
    ../Mod/Part/App/FeatureRevolution.cpp
    ../Mod/Part/App/FeatureRevolution.h
    ../Mod/Part/App/FeatureOffset.cpp
    ../Mod/Part/App/FeatureOffset.h
    ../Mod/Part/App/PartFeatures.cpp
    ../Mod/Part/App/PartFeatures.h
    ../Mod/Part/App/PartFeature.cpp
    ../Mod/Part/App/PartFeature.h
    ../Mod/Part/App/PartFeatureReference.cpp
    ../Mod/Part/App/PartFeatureReference.h
    ../Mod/Part/App/Part2DObject.cpp
    ../Mod/Part/App/Part2DObject.h
    ../Mod/Part/App/PrimitiveFeature.cpp
    ../Mod/Part/App/PrimitiveFeature.h
    ../Mod/Part/App/FeatureGeometrySet.h
    ../Mod/Part/App/FeatureGeometrySet.cpp
    ../Mod/Part/App/CustomFeature.cpp
    ../Mod/Part/App/CustomFeature.h
    ../Mod/Part/App/BodyBase.h
    ../Mod/Part/App/BodyBase.cpp
    ../Mod/Part/App/DatumFeature.cpp
    ../Mod/Part/App/DatumFeature.h
    ../Mod/Part/App/AttachExtension.h
    ../Mod/Part/App/AttachExtension.cpp
)
SET(Properties_SRCS
    ../Mod/Part/App/PropertyTopoShape.cpp
    ../Mod/Part/App/PropertyTopoShape.h
    ../Mod/Part/App/PropertyGeometryList.cpp
    ../Mod/Part/App/PropertyGeometryList.h
)

SET(Part_SRCS
    ${Features_SRCS}
    ${Properties_SRCS}
#    ${Python_SRCS}
#    ${Geom2dPy_SRCS}
    ../Mod/Part/App/Attacher.cpp
    ../Mod/Part/App/Attacher.h
    ../Mod/Part/App/AppPart.cpp
    ../Mod/Part/App/AppPartPy.cpp
    ../Mod/Part/App/BSplineCurveBiArcs.cpp
    ../Mod/Part/App/CrossSection.cpp
    ../Mod/Part/App/CrossSection.h
    ../Mod/Part/App/Geometry.cpp
    ../Mod/Part/App/Geometry.h
    ../Mod/Part/App/Geometry2d.cpp
    ../Mod/Part/App/Geometry2d.h
    ../Mod/Part/App/ImportIges.cpp
    ../Mod/Part/App/ImportIges.h
    ../Mod/Part/App/ImportStep.cpp
    ../Mod/Part/App/ImportStep.h
    ../Mod/Part/App/PreCompiled.cpp
    ../Mod/Part/App/PreCompiled.h
    ../Mod/Part/App/ProgressIndicator.cpp
    ../Mod/Part/App/ProgressIndicator.h
    ../Mod/Part/App/TopoShape.cpp
    ../Mod/Part/App/TopoShape.h
    ../Mod/Part/App/edgecluster.cpp
    ../Mod/Part/App/edgecluster.h
    ../Mod/Part/App/modelRefine.cpp
    ../Mod/Part/App/modelRefine.h
    ../Mod/Part/App/Tools.cpp
    ../Mod/Part/App/Tools.h
    ../Mod/Part/App/encodeFilename.h
    ../Mod/Part/App/OCCError.h
    ../Mod/Part/App/FT2FC.cpp
    ../Mod/Part/App/FT2FC.h
    ../Mod/Part/App/FaceMaker.cpp
    ../Mod/Part/App/FaceMaker.h
    ../Mod/Part/App/FaceMakerCheese.cpp
    ../Mod/Part/App/FaceMakerCheese.h
    ../Mod/Part/App/FaceMakerBullseye.cpp
    ../Mod/Part/App/FaceMakerBullseye.h
)

set(PartGui_MOC_HDRS
    ../Mod/Part/Gui/CrossSections.h
    ../Mod/Part/Gui/Mirroring.h
    ../Mod/Part/Gui/DlgBooleanOperation.h
    ../Mod/Part/Gui/DlgExtrusion.h
    ../Mod/Part/Gui/DlgFilletEdges.h
    ../Mod/Part/Gui/DlgPartBoxImp.h
    ../Mod/Part/Gui/DlgPartCylinderImp.h
    ../Mod/Part/Gui/DlgPartImportIgesImp.h
    ../Mod/Part/Gui/DlgPartImportStepImp.h
    ../Mod/Part/Gui/DlgPrimitives.h
    ../Mod/Part/Gui/DlgRevolution.h
    ../Mod/Part/Gui/DlgSettings3DViewPartImp.h
    ../Mod/Part/Gui/DlgSettingsGeneral.h
    ../Mod/Part/Gui/DlgSettingsObjectColor.h
    ../Mod/Part/Gui/TaskFaceColors.h
    ../Mod/Part/Gui/TaskShapeBuilder.h
    ../Mod/Part/Gui/TaskLoft.h
    ../Mod/Part/Gui/TaskOffset.h
    ../Mod/Part/Gui/TaskSweep.h
    ../Mod/Part/Gui/TaskThickness.h
    ../Mod/Part/Gui/TaskDimension.h
    ../Mod/Part/Gui/TaskCheckGeometry.h
    ../Mod/Part/Gui/TaskAttacher.h
)
fc_wrap_cpp(PartGui_MOC_SRCS ${PartGui_MOC_HDRS})
SOURCE_GROUP("Moc" FILES ${PartGui_MOC_SRCS})

if (BUILD_QT5)
    qt5_add_resources(PartGui_QRC_SRCS ../Mod/Part/Gui/Resources/Part.qrc)
else()
    qt4_add_resources(PartGui_QRC_SRCS ../Mod/Part/Gui/Resources/Part.qrc)
endif()

set(PartGui_UIC_SRCS
    ../Mod/Part/Gui/CrossSections.ui
    ../Mod/Part/Gui/Mirroring.ui
    ../Mod/Part/Gui/DlgBooleanOperation.ui
    ../Mod/Part/Gui/DlgExtrusion.ui
    ../Mod/Part/Gui/DlgFilletEdges.ui
    ../Mod/Part/Gui/DlgImportExportIges.ui
    ../Mod/Part/Gui/DlgImportExportStep.ui
    ../Mod/Part/Gui/DlgPartBox.ui
    ../Mod/Part/Gui/DlgPartCylinder.ui
    ../Mod/Part/Gui/DlgPartImportIges.ui
    ../Mod/Part/Gui/DlgPartImportStep.ui
    ../Mod/Part/Gui/DlgPrimitives.ui
    ../Mod/Part/Gui/Location.ui
    ../Mod/Part/Gui/DlgRevolution.ui
    ../Mod/Part/Gui/DlgSettings3DViewPart.ui
    ../Mod/Part/Gui/DlgSettingsGeneral.ui
    ../Mod/Part/Gui/DlgSettingsObjectColor.ui
    ../Mod/Part/Gui/TaskFaceColors.ui
    ../Mod/Part/Gui/TaskShapeBuilder.ui
    ../Mod/Part/Gui/TaskLoft.ui
    ../Mod/Part/Gui/TaskOffset.ui
    ../Mod/Part/Gui/TaskSweep.ui
    ../Mod/Part/Gui/TaskAttacher.ui
)

if (BUILD_QT5)
    qt5_wrap_ui(PartGui_UIC_HDRS ${PartGui_UIC_SRCS})
else()
    qt4_wrap_ui(PartGui_UIC_HDRS ${PartGui_UIC_SRCS})
endif()


SET(PartGui_SRCS
    ${PartGui_QRC_SRCS}
    ${PartGui_UIC_HDRS}
    ../Mod/Part/Gui/AppPartGui.cpp
    ../Mod/Part/Gui/AttacherTexts.h
    ../Mod/Part/Gui/AttacherTexts.cpp
    ../Mod/Part/Gui/Command.cpp
    ../Mod/Part/Gui/CommandSimple.cpp
    ../Mod/Part/Gui/CommandParametric.cpp
    ../Mod/Part/Gui/CrossSections.cpp
    ../Mod/Part/Gui/CrossSections.h
    ../Mod/Part/Gui/CrossSections.ui
    ../Mod/Part/Gui/Mirroring.cpp
    ../Mod/Part/Gui/Mirroring.h
    ../Mod/Part/Gui/Mirroring.ui
    ../Mod/Part/Gui/DlgBooleanOperation.cpp
    ../Mod/Part/Gui/DlgBooleanOperation.h
    ../Mod/Part/Gui/DlgBooleanOperation.ui
    ../Mod/Part/Gui/DlgExtrusion.cpp
    ../Mod/Part/Gui/DlgExtrusion.h
    ../Mod/Part/Gui/DlgExtrusion.ui
    ../Mod/Part/Gui/DlgFilletEdges.cpp
    ../Mod/Part/Gui/DlgFilletEdges.h
    ../Mod/Part/Gui/DlgFilletEdges.ui
    ../Mod/Part/Gui/DlgImportExportIges.ui
    ../Mod/Part/Gui/DlgImportExportStep.ui
    ../Mod/Part/Gui/DlgPartBox.ui
    ../Mod/Part/Gui/DlgPartBoxImp.cpp
    ../Mod/Part/Gui/DlgPartBoxImp.h
    ../Mod/Part/Gui/DlgPartCylinderImp.cpp
    ../Mod/Part/Gui/DlgPartCylinderImp.h
    ../Mod/Part/Gui/DlgPartCylinder.ui
    ../Mod/Part/Gui/DlgPartImportIges.ui
    ../Mod/Part/Gui/DlgPartImportIgesImp.cpp
    ../Mod/Part/Gui/DlgPartImportIgesImp.h
    ../Mod/Part/Gui/DlgPartImportStep.ui
    ../Mod/Part/Gui/DlgPartImportStepImp.cpp
    ../Mod/Part/Gui/DlgPartImportStepImp.h
    ../Mod/Part/Gui/DlgPrimitives.cpp
    ../Mod/Part/Gui/DlgPrimitives.h
    ../Mod/Part/Gui/DlgPrimitives.ui
    ../Mod/Part/Gui/Location.ui
    ../Mod/Part/Gui/DlgRevolution.cpp
    ../Mod/Part/Gui/DlgRevolution.h
    ../Mod/Part/Gui/DlgRevolution.ui
    ../Mod/Part/Gui/DlgSettings3DViewPart.ui
    ../Mod/Part/Gui/DlgSettings3DViewPartImp.cpp
    ../Mod/Part/Gui/DlgSettings3DViewPartImp.h
    ../Mod/Part/Gui/DlgSettingsGeneral.cpp
    ../Mod/Part/Gui/DlgSettingsGeneral.h
    ../Mod/Part/Gui/DlgSettingsGeneral.ui
    ../Mod/Part/Gui/DlgSettingsObjectColor.cpp
    ../Mod/Part/Gui/DlgSettingsObjectColor.h
    ../Mod/Part/Gui/DlgSettingsObjectColor.ui
    ../Mod/Part/Gui/Resources/Part.qrc
    ../Mod/Part/Gui/PreCompiled.cpp
    ../Mod/Part/Gui/PreCompiled.h
    ../Mod/Part/Gui/SoFCShapeObject.cpp
    ../Mod/Part/Gui/SoFCShapeObject.h
    ../Mod/Part/Gui/SoBrepEdgeSet.cpp
    ../Mod/Part/Gui/SoBrepEdgeSet.h
    ../Mod/Part/Gui/SoBrepFaceSet.cpp
    ../Mod/Part/Gui/SoBrepFaceSet.h
    ../Mod/Part/Gui/SoBrepPointSet.cpp
    ../Mod/Part/Gui/SoBrepPointSet.h
    ../Mod/Part/Gui/ViewProvider.cpp
    ../Mod/Part/Gui/ViewProvider.h
    ../Mod/Part/Gui/ViewProviderExt.cpp
    ../Mod/Part/Gui/ViewProviderExt.h
    ../Mod/Part/Gui/ViewProviderReference.cpp
    ../Mod/Part/Gui/ViewProviderReference.h
    ../Mod/Part/Gui/ViewProviderBox.cpp
    ../Mod/Part/Gui/ViewProviderBox.h
    ../Mod/Part/Gui/ViewProviderCompound.cpp
    ../Mod/Part/Gui/ViewProviderCompound.h
    ../Mod/Part/Gui/ViewProviderCircleParametric.cpp
    ../Mod/Part/Gui/ViewProviderCircleParametric.h
    ../Mod/Part/Gui/ViewProviderLineParametric.cpp
    ../Mod/Part/Gui/ViewProviderLineParametric.h
    ../Mod/Part/Gui/ViewProviderPointParametric.cpp
    ../Mod/Part/Gui/ViewProviderPointParametric.h
    ../Mod/Part/Gui/ViewProviderEllipseParametric.cpp
    ../Mod/Part/Gui/ViewProviderEllipseParametric.h
    ../Mod/Part/Gui/ViewProviderHelixParametric.cpp
    ../Mod/Part/Gui/ViewProviderHelixParametric.h
    ../Mod/Part/Gui/ViewProviderPlaneParametric.cpp
    ../Mod/Part/Gui/ViewProviderPlaneParametric.h
    ../Mod/Part/Gui/ViewProviderSphereParametric.cpp
    ../Mod/Part/Gui/ViewProviderSphereParametric.h
    ../Mod/Part/Gui/ViewProviderCylinderParametric.cpp
    ../Mod/Part/Gui/ViewProviderCylinderParametric.h
    ../Mod/Part/Gui/ViewProviderConeParametric.cpp
    ../Mod/Part/Gui/ViewProviderConeParametric.h
    ../Mod/Part/Gui/ViewProviderPrism.cpp
    ../Mod/Part/Gui/ViewProviderPrism.h
    ../Mod/Part/Gui/ViewProviderRegularPolygon.cpp
    ../Mod/Part/Gui/ViewProviderRegularPolygon.h
    ../Mod/Part/Gui/ViewProviderTorusParametric.cpp
    ../Mod/Part/Gui/ViewProviderTorusParametric.h
    ../Mod/Part/Gui/ViewProviderCurveNet.cpp
    ../Mod/Part/Gui/ViewProviderCurveNet.h
    ../Mod/Part/Gui/ViewProviderSpline.cpp
    ../Mod/Part/Gui/ViewProviderSpline.h
    ../Mod/Part/Gui/ViewProviderImport.cpp
    ../Mod/Part/Gui/ViewProviderImport.h
    ../Mod/Part/Gui/ViewProviderExtrusion.cpp
    ../Mod/Part/Gui/ViewProviderExtrusion.h
    ../Mod/Part/Gui/ViewProvider2DObject.cpp
    ../Mod/Part/Gui/ViewProvider2DObject.h
    ../Mod/Part/Gui/ViewProviderMirror.cpp
    ../Mod/Part/Gui/ViewProviderMirror.h
    ../Mod/Part/Gui/ViewProviderPython.cpp
    ../Mod/Part/Gui/ViewProviderPython.h
    ../Mod/Part/Gui/ViewProviderBoolean.cpp
    ../Mod/Part/Gui/ViewProviderBoolean.h
    ../Mod/Part/Gui/ViewProviderRuledSurface.cpp
    ../Mod/Part/Gui/ViewProviderRuledSurface.h
    ../Mod/Part/Gui/Workbench.cpp
    ../Mod/Part/Gui/Workbench.h
    ../Mod/Part/Gui/TaskFaceColors.cpp
    ../Mod/Part/Gui/TaskFaceColors.h
    ../Mod/Part/Gui/TaskFaceColors.ui
    ../Mod/Part/Gui/TaskShapeBuilder.cpp
    ../Mod/Part/Gui/TaskShapeBuilder.h
    ../Mod/Part/Gui/TaskShapeBuilder.ui
    ../Mod/Part/Gui/TaskLoft.cpp
    ../Mod/Part/Gui/TaskLoft.h
    ../Mod/Part/Gui/TaskLoft.ui
    ../Mod/Part/Gui/TaskOffset.cpp
    ../Mod/Part/Gui/TaskOffset.h
    ../Mod/Part/Gui/TaskOffset.ui
    ../Mod/Part/Gui/TaskSweep.cpp
    ../Mod/Part/Gui/TaskSweep.h
    ../Mod/Part/Gui/TaskSweep.ui
    ../Mod/Part/Gui/TaskThickness.cpp
    ../Mod/Part/Gui/TaskThickness.h
    ../Mod/Part/Gui/TaskDimension.cpp
    ../Mod/Part/Gui/TaskDimension.h
    ../Mod/Part/Gui/TaskCheckGeometry.cpp
    ../Mod/Part/Gui/TaskCheckGeometry.h
    ../Mod/Part/Gui/TaskAttacher.h 
    ../Mod/Part/Gui/TaskAttacher.cpp 
)

SET(FreeCADGui_SRCS
    ${FreeCADGui_SDK_SRCS}
    ${FreeCADGui_CPP_SRCS}
    ${FreeCADGui_XML_SRCS}
    ${iis_MOC_SRCS}
    ${qsint_MOC_SRCS}
    ${Gui_QRC_SRCS}
    ${Gui_UIC_HDRS}
    ${Command_SRCS}
    ${Dialog_SRCS}
    ${Dialog_Customize_SRCS}
    ${Dialog_Settings_SRCS}
    ${Dock_Windows_SRCS}
    ${Editor_SRCS}
    ${Help_SRCS}
    ${Inventor_SRCS}
    ${Language_SRCS}
    ${Propertyeditor_SRCS}
    ${Task_View_SRCS}
    ${iisTaskPanel_SRCS}
    ${qsintActionPanel_SRCS}
    ${Resource_SRCS}
    ${Quarter_SRCS}
    ${View3D_SRCS}
    ${Viewprovider_SRCS}
    ${Widget_SRCS}
    ${View_SRCS}
    ${Workbench_SRCS}
    ${Selection_SRCS}
    ${FreeCADGui_SRCS}
    ${Part_SRCS}
    ${PartGui_SRCS}
)



SET(FreeCADBase_SRCS
    ${PYCXX_SOURCES}
    ${FreeCADBase_CPP_SRCS}
    ${FreeCADBase_HPP_SRCS}
    ${FreeCADBase_XML_SRCS}
    ${FreeCADBase_UNITAPI_SRCS}
    ${Part_SRCS}
    ${PartGui_SRCS}
    ${FreeCADApp_SRCS}
    ${FreeCADGui_SRCS}
    PyTools.c
    PyTools.h
    PreCompiled.cpp
    PreCompiled.h
)

IF (MSVC)
    SET(FreeCADBase_SRCS
        ${FreeCADBase_SRCS}
        StackWalker.cpp
        StackWalker.h
)
ENDIF(MSVC)

# Use external zipios++ if specified.
if(FREECAD_USE_EXTERNAL_ZIPIOS)
    find_library(ZIPIOS_LIBRARY zipios)
    find_path(ZIPIOS_INCLUDES zipios++/zipios-config.h)
    if(ZIPIOS_LIBRARY)
        message(STATUS "Found zipios++: ${ZIPIOS}")
    endif()
    if(ZIPIOS_INCLUDES)
        message(STATUS "Found zipios++ headers.")
    endif()
    if(ZIPIOS_LIBRARY AND ZIPIOS_INCLUDES)
        list(APPEND FreeCADBase_LIBS ${ZIPIOS_LIBRARY})
        include_directories(${ZIPIOS_INCLUDES})
    else()
        message(FATAL_ERROR "Using external zipios++ was specified but was not found.")
    endif()
else(FREECAD_USE_EXTERNAL_ZIPIOS)
    list(APPEND FreeCADBase_SRCS ${zipios_SRCS})
endif(FREECAD_USE_EXTERNAL_ZIPIOS)


if(MSVC)
add_definitions(-D_PreComp_)
ADD_MSVC_PRECOMPILED_HEADER(FreeCADBase PreCompiled.h PreCompiled.cpp FreeCADBase_CPP_SRCS)
endif(MSVC)

add_library(FreeCADBase SHARED ${FreeCADBase_SRCS})

target_link_libraries(FreeCADBase ${FreeCADBase_LIBS})

SET_BIN_DIR(FreeCADBase FreeCADBase)

if(WIN32)
    INSTALL(TARGETS FreeCADBase
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
else(WIN32)
    INSTALL(TARGETS FreeCADBase
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif(WIN32)
