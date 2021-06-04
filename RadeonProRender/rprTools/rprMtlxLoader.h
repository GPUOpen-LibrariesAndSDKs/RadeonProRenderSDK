/*****************************************************************************\
*
*  Module Name    rprMtlxLoader.h
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#ifndef RPRTOOLS_MTLX_LOADER_H
#define RPRTOOLS_MTLX_LOADER_H

#include <RadeonProRender.h>

#include <MaterialXCore/Document.h>
#include <MaterialXFormat/File.h>

class RPRMtlxLoader {
public:
    RPRMtlxLoader();

    void SetupStdlib(MaterialX::FilePathVec const& libraryNames, MaterialX::FileSearchPath const& searchPath);
    MaterialX::ConstDocumentPtr GetStdlib() const { return _stdlib; }

    void SetLogging(bool enable) { _loggingEnabled = enable; }

    enum OutputType {
        kOutputNone = -1,
        kOutputSurface,
        kOutputDisplacement,
        kOutputsTotal,
        kOutputAny = kOutputsTotal
    };

    struct RenderableElements {
        /// A range of name paths to renderable elements per OutputType,
        /// an actual MaterialX::Element may be resolved with MaterialX::Document::getDescendant
        std::vector<std::string> namePaths[kOutputsTotal];
    };
    static RenderableElements GetRenderableElements(MaterialX::Document const* mtlxDocument);

    struct Result {
        /// All rpr nodes that form a material graph
        rpr_material_node* nodes = nullptr;

        /// Number of elements in nodes array
        size_t numNodes;

        size_t rootNodeIndices[kOutputsTotal];
        static const size_t kInvalidRootNodeIndex = size_t(-1);

        struct ImageNode {
            /// The URI of an image file.
            /// It's responsibility of the loader user to setup rpr_image and bind it to rprNode
            std::string file;

            /// The name of the layer to extract from a multi-layer input file
            std::string layer;

            /// A default value to use if the file reference can not be resolved
            MaterialX::ValuePtr defaultValue;

            /// Determines how U/V coordinates outside the 0-1 range are processed.
            /// Possible values: "constant", "clamp", "periodic", "mirror"
            std::string uaddressmode;
            std::string vaddressmode;

            /// The target image texture node
            rpr_material_node rprNode;
        };

        ImageNode* imageNodes = nullptr;
        size_t numImageNodes;
    };

    /// \ref Load parses provided \p mtlxDocument;
    ///
    /// \p renderableElements controls what element from \p mtlxDocument to use for each OutputType.
    /// \p renderableElements may be null, in this case, loader will use first available ones in the mtlxDocument.
    /// To disable a particular output type element, pass empty string.
    /// If any renderable element (non-empty string) does not exist in mtlxDocument,
    /// the loader will auto-select a renderable element with a corresponding output type.
    ///
    /// All parameters of `filename` type is resolved by the loader,
    /// part of the resolve process consists of searching file in a particular directories,
    /// RPRMtlxLoader's default search paths:
    ///   * value of MATERIALX_SEARCH_PATH env.var.
    ///   * stdlib search paths passed to RPRMtlxLoader::SetupStdlib
    /// \p searchPath is appended to these paths
    Result Load(
        MaterialX::Document const* mtlxDocument,
        const std::string inputRenderableElements[kOutputsTotal],
        MaterialX::FileSearchPath const& searchPath,
        rpr_material_system rprMatSys);

    /// Reference function on how properly to release RPRMtlxLoader::Result
    static void Release(Result* result) {
        if (!result || !result->nodes) {
            return;
        }

        for (size_t i = 0; i < result->numNodes; ++i) {
            if (result->nodes[i]) {
                rprObjectDelete(result->nodes[i]);
            }
        }
        delete[] result->nodes;
        if (result->imageNodes) {
            delete[] result->imageNodes;
        }

        *result = Result{};
    }

private:
    MaterialX::DocumentPtr _stdlib;
    MaterialX::FileSearchPath _stdSearchPath;
    bool _loggingEnabled = false;
};

#endif // RPRTOOLS_MTLX_LOADER_H
