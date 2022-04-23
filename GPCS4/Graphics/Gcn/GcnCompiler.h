#pragma once

#include "GcnCommon.h"
#include "GcnProgramInfo.h"
#include "GcnInstructionIterator.h"
#include "GcnShaderMeta.h"
#include "GcnDecoder.h"
#include "GcnStateRegister.h"
#include "GcnCompilerDefs.h"

#include "SpirV/SpirvModule.h"
#include "Violet/VltRc.h"
#include "Violet/VltPipeLayout.h"
#include "Violet/VltShader.h"

#include <vector>

namespace sce::Gnm
{
	enum TextureType;
}  // namespace sce::Gnm

namespace sce::gcn
{

	class GcnHeader;
	struct GcnShaderInstruction;
	struct GcnAnalysisInfo;
	struct GcnShaderResource;


	/**
	 * \brief Shader recompiler
	 * 
	 * Recompile GCN instructions into Spir-V byte code.
	 * Produce VltShader for Violet.
	 */
	class GcnCompiler : public GcnInstructionIterator
	{
		friend class GcnStateRegister;
	public:
		GcnCompiler(
			const std::string&     fileName,
			const GcnProgramInfo&  programInfo,
			const GcnHeader&       header,
			const GcnShaderMeta&   meta,
			const GcnAnalysisInfo& analysis);
		virtual ~GcnCompiler();

		/**
         * \brief Processes a single instruction
         * \param [in] ins The instruction
         */
		virtual void processInstruction(
			const GcnShaderInstruction& ins) override;

		/**
         * \brief Finalizes the shader
         * \returns The final shader object
         */
		vlt::Rc<vlt::VltShader> finalize();

	private:
		void compileInstruction(
			const GcnShaderInstruction& ins);
		/////////////////////////////////////////////////////////
		// Category handlers
		void emitScalarALU(const GcnShaderInstruction& ins);
		void emitScalarMemory(const GcnShaderInstruction& ins);
		void emitVectorALU(const GcnShaderInstruction& ins);
		void emitVectorMemory(const GcnShaderInstruction& ins);
		void emitFlowControl(const GcnShaderInstruction& ins);
		void emitDataShare(const GcnShaderInstruction& ins);
		void emitVectorInterpolation(const GcnShaderInstruction& ins);
		void emitExport(const GcnShaderInstruction& ins);
		void emitDebugProfile(const GcnShaderInstruction& ins);

		/////////////////////////////////////////////////////////
		// Class handlers
		void emitScalarArith(const GcnShaderInstruction& ins);
		void emitScalarAbs(const GcnShaderInstruction& ins);
		void emitScalarMov(const GcnShaderInstruction& ins);
		void emitScalarCmp(const GcnShaderInstruction& ins);
		void emitScalarSelect(const GcnShaderInstruction& ins);
		void emitScalarBitLogic(const GcnShaderInstruction& ins);
		void emitScalarBitManip(const GcnShaderInstruction& ins);
		void emitScalarBitField(const GcnShaderInstruction& ins);
		void emitScalarConv(const GcnShaderInstruction& ins);
		void emitScalarExecMask(const GcnShaderInstruction& ins);
		void emitScalarQuadMask(const GcnShaderInstruction& ins);
		void emitVectorRegMov(const GcnShaderInstruction& ins);
		void emitVectorLane(const GcnShaderInstruction& ins);
		void emitVectorBitLogic(const GcnShaderInstruction& ins);
		void emitVectorBitField32(const GcnShaderInstruction& ins);
		void emitVectorThreadMask(const GcnShaderInstruction& ins);
		void emitVectorBitField64(const GcnShaderInstruction& ins);
		void emitVectorFpArith32(const GcnShaderInstruction& ins);
		void emitVectorFpRound32(const GcnShaderInstruction& ins);
		void emitVectorFpField32(const GcnShaderInstruction& ins);
		void emitVectorFpTran32(const GcnShaderInstruction& ins);
		void emitVectorFpCmp32(const GcnShaderInstruction& ins);
		void emitVectorFpArith64(const GcnShaderInstruction& ins);
		void emitVectorFpRound64(const GcnShaderInstruction& ins);
		void emitVectorFpField64(const GcnShaderInstruction& ins);
		void emitVectorFpTran64(const GcnShaderInstruction& ins);
		void emitVectorFpCmp64(const GcnShaderInstruction& ins);
		void emitVectorIntArith32(const GcnShaderInstruction& ins);
		void emitVectorIntArith64(const GcnShaderInstruction& ins);
		void emitVectorIntCmp32(const GcnShaderInstruction& ins);
		void emitVectorIntCmp64(const GcnShaderInstruction& ins);
		void emitVectorConv(const GcnShaderInstruction& ins);
		void emitVectorFpGraph32(const GcnShaderInstruction& ins);
		void emitVectorIntGraph(const GcnShaderInstruction& ins);
		void emitVectorMisc(const GcnShaderInstruction& ins);
		void emitScalarProgFlow(const GcnShaderInstruction& ins);
		void emitScalarSync(const GcnShaderInstruction& ins);
		void emitScalarWait(const GcnShaderInstruction& ins);
		void emitScalarCache(const GcnShaderInstruction& ins);
		void emitScalarPrior(const GcnShaderInstruction& ins);
		void emitScalarRegAccess(const GcnShaderInstruction& ins);
		void emitScalarMsg(const GcnShaderInstruction& ins);
		void emitScalarMemRd(const GcnShaderInstruction& ins);
		void emitScalarMemUt(const GcnShaderInstruction& ins);
		void emitVectorMemBufNoFmt(const GcnShaderInstruction& ins);
		void emitVectorMemBufFmt(const GcnShaderInstruction& ins);
		void emitVectorMemImgNoSmp(const GcnShaderInstruction& ins);
		void emitVectorMemImgSmp(const GcnShaderInstruction& ins);
		void emitVectorMemImgUt(const GcnShaderInstruction& ins);
		void emitVectorMemL1Cache(const GcnShaderInstruction& ins);
		void emitDsIdxRd(const GcnShaderInstruction& ins);
		void emitDsIdxWr(const GcnShaderInstruction& ins);
		void emitDsIdxWrXchg(const GcnShaderInstruction& ins);
		void emitDsIdxCondXchg(const GcnShaderInstruction& ins);
		void emitDsIdxWrap(const GcnShaderInstruction& ins);
		void emitDsAtomicArith32(const GcnShaderInstruction& ins);
		void emitDsAtomicArith64(const GcnShaderInstruction& ins);
		void emitDsAtomicMinMax32(const GcnShaderInstruction& ins);
		void emitDsAtomicMinMax64(const GcnShaderInstruction& ins);
		void emitDsAtomicCmpSt32(const GcnShaderInstruction& ins);
		void emitDsAtomicCmpSt64(const GcnShaderInstruction& ins);
		void emitDsAtomicLogic32(const GcnShaderInstruction& ins);
		void emitDsAtomicLogic64(const GcnShaderInstruction& ins);
		void emitDsAppendCon(const GcnShaderInstruction& ins);
		void emitDsDataShareUt(const GcnShaderInstruction& ins);
		void emitDsDataShareMisc(const GcnShaderInstruction& ins);
		void emitGdsSync(const GcnShaderInstruction& ins);
		void emitGdsOrdCnt(const GcnShaderInstruction& ins);
		void emitVectorInterpFpCache(const GcnShaderInstruction& ins);
		void emitExp(const GcnShaderInstruction& ins);
		void emitDbgPro(const GcnShaderInstruction& ins);
		//////////////////////////////////////
		// Common function definition methods
		void emitInit();

		void emitFunctionBegin(
			uint32_t entryPoint,
			uint32_t returnType,
			uint32_t funcType);

		void emitFunctionEnd();

		void emitFunctionLabel();

		void emitMainFunctionBegin();

		/////////////////////////////////
		// Shader initialization methods
		void emitVsInit();
		void emitHsInit();
		void emitDsInit();
		void emitGsInit();
		void emitPsInit();
		void emitCsInit();

		///////////////////////////////
		// Shader finalization methods
		void emitVsFinalize();
		void emitHsFinalize();
		void emitDsFinalize();
		void emitGsFinalize();
		void emitPsFinalize();
		void emitCsFinalize();

		////////////////////////////
		// Input/output preparation
		void emitInputSetup();
		void emitFetchInput();

		void emitOutputSetup();
		/////////////////////////////////////////////////////
		// Shader interface and metadata declaration methods
		void emitDclInputSlots();
		void emitDclVertexInput();
		void emitDclInput(
			const VertexInputSemantic& sema);

		void emitDclBuffer(
			const GcnShaderResource& res);
		void emitDclTexture(
			const GcnShaderResource& res);
		void emitDclSampler(
			const GcnShaderResource& res);
		///////////////////////////////
		// Variable definition methods
		uint32_t emitNewVariable(
			const GcnRegisterInfo& info);

		uint32_t emitNewBuiltinVariable(
			const GcnRegisterInfo& info,
			spv::BuiltIn           builtIn,
			const char*            name);

		///////////////////////////////
		// SGPR/VGPR load/store methods
		template <bool IsVgpr>
		GcnRegisterValue emitGprLoad(
			const GcnInstOperand& reg);
		GcnRegisterValue emitVgprLoad(
			const GcnInstOperand& reg);
		GcnRegisterValue emitSgprLoad(
			const GcnInstOperand& reg);
		template <bool IsVgpr>
		GcnRegisterValue emitGprArrayLoad(
			const GcnInstOperand& start,
			uint32_t              count);
		GcnRegisterValue emitVgprArrayLoad(
			const GcnInstOperand& start,
			uint32_t              count);
		GcnRegisterValue emitSgprArrayLoad(
			const GcnInstOperand& start,
			uint32_t              count);

		template <bool IsVgpr>
		void emitGprStore(
			const GcnInstOperand&   reg,
			const GcnRegisterValue& value);
		void emitVgprStore(
			const GcnInstOperand&   reg,
			const GcnRegisterValue& value);
		void emitSgprStore(
			const GcnInstOperand&   reg,
			const GcnRegisterValue& value);
		template <bool IsVgpr>
		void emitGprArrayStore(
			const GcnInstOperand&   start,
			uint32_t                count,
			const GcnRegisterValue& value);
		void emitVgprArrayStore(
			const GcnInstOperand&   start,
			uint32_t                count,
			const GcnRegisterValue& value);
		void emitSgprArrayStore(
			const GcnInstOperand&   start,
			uint32_t                count,
			const GcnRegisterValue& value);

		//////////////////////////////
		// Operand load/store methods
		GcnRegisterValue emitValueLoad(
			GcnRegisterPointer ptr);

		void emitValueStore(
			GcnRegisterPointer ptr,
			GcnRegisterValue   value,
			GcnRegMask         writeMask);

		GcnRegisterValuePair emitRegisterLoad(
			const GcnInstOperand& reg);

		void emitRegisterStore(
			const GcnInstOperand&       reg,
			const GcnRegisterValuePair& value);

		GcnRegisterPointer emitCompositeAccess(
			GcnRegisterPointer pointer,
			spv::StorageClass  sclass,
			uint32_t           index);

		////////////////////////////////////////////////
		// Constant building methods. These are used to
		// generate constant vectors that store the same
		// value in each component.
		GcnRegisterValue emitBuildConstVecf32(
			float              x,
			float              y,
			float              z,
			float              w,
			const GcnRegMask& writeMask);

		GcnRegisterValue emitBuildConstVecu32(
			uint32_t           x,
			uint32_t           y,
			uint32_t           z,
			uint32_t           w,
			const GcnRegMask& writeMask);

		GcnRegisterValue emitBuildConstVeci32(
			int32_t            x,
			int32_t            y,
			int32_t            z,
			int32_t            w,
			const GcnRegMask& writeMask);

		GcnRegisterValue emitBuildConstVecf64(
			double             xy,
			double             zw,
			const GcnRegMask& writeMask);
		/////////////////////////////////////////
		// Generic register manipulation methods
		GcnRegisterValue emitRegisterBitcast(
			GcnRegisterValue srcValue,
			GcnScalarType    dstType);

		GcnRegisterValuePair emitRegisterBitcast(
			GcnRegisterValuePair srcValue,
			GcnScalarType        dstType);

		GcnRegisterValue emitRegisterSwizzle(
			GcnRegisterValue value,
			GcnRegSwizzle    swizzle,
			GcnRegMask       writeMask);

		GcnRegisterValue emitRegisterExtract(
			GcnRegisterValue value,
			GcnRegMask       mask);

		GcnRegisterValue emitRegisterInsert(
			GcnRegisterValue dstValue,
			GcnRegisterValue srcValue,
			GcnRegMask       srcMask);

		GcnRegisterValue emitRegisterConcat(
			GcnRegisterValue value1,
			GcnRegisterValue value2);

		GcnRegisterValue emitRegisterExtend(
			GcnRegisterValue value,
			uint32_t          size);

		GcnRegisterValue emitRegisterAbsolute(
			GcnRegisterValue value);

		GcnRegisterValue emitRegisterNegate(
			GcnRegisterValue value);

		GcnRegisterValue emitRegisterZeroTest(
			GcnRegisterValue value,
			GcnZeroTest      test);

		GcnRegisterValue emitRegisterMaskBits(
			GcnRegisterValue value,
			uint32_t          mask);

		//GcnRegisterValue emitSrcOperandModifiers(
		//	GcnRegisterValue value,
		//	GcnRegModifiers  modifiers);

		//GcnRegisterValue emitDstOperandModifiers(
		//	GcnRegisterValue value,
		//	GcnOpModifiers   modifiers);

		///////////////////////////
		// Type definition methods
		uint32_t getScalarTypeId(
			GcnScalarType type);

		uint32_t getVectorTypeId(
			const GcnVectorType& type);

		uint32_t getArrayTypeId(
			const GcnArrayType& type);

		uint32_t getPointerTypeId(
			const GcnRegisterInfo& type);

		uint32_t getPerVertexBlockId();


		///////////////////////////
		//
		bool isDoubleType(
			GcnScalarType type) const;

		uint32_t getUserSgprCount() const;

		bool hasFetchShader() const;

		std::pair<const VertexInputSemantic*, uint32_t>
			getSemanticTable();

		const std::array<GcnTextureInfo, 128>&
			getTextureInfoTable();

		GcnImageInfo getImageType(
			Gnm::TextureType textureType,
			bool             isStorage,
			bool             isDepth) const;

	private:
		GcnProgramInfo         m_programInfo;
		const GcnHeader*       m_header;
		GcnShaderMeta          m_meta;
		const GcnAnalysisInfo* m_analysis;
		SpirvModule            m_module;

		// Hardware state registers.
		GcnStateRegisters m_state;
		///////////////////////////////////////////////////
		// Entry point description - we'll need to declare
		// the function ID and all input/output variables.
		uint32_t              m_entryPointId = 0;
		std::vector<uint32_t> m_entryPointInterfaces;

		///////////////////////////////////////////////////
		// Shader input interfaces
		std::array<
			GcnRegisterPointer, 
			GcnMaxInterfaceRegs> m_inputs;

		///////////////////////////////////////////////////
		// VGPR/SGPR registers
		std::array<
			GcnRegisterPointer,
			GcnMaxVGPR> m_vgprs = {};
		std::array<
			GcnRegisterPointer,
			GcnMaxSGPR> m_sgprs = {};

		//////////////////////////////////////////////////////
		// Shader resource variables. These provide access to
		// buffers, samplers and textures.
		std::array<GcnBuffer, 16>   m_buffers;
		std::array<GcnSampler, 16>  m_samplers;
		std::array<GcnTexture, 128> m_textures;

		//////////////////////////////////////////////
		// Function state tracking. Required in order
		// to properly end functions in some cases.
		bool m_insideFunction = false;
		///////////////////////////////////////////////////////////
		// An array stores up to 16 user data registers.
		uint32_t m_vUserDataArray = 0;

		////////////////////////////////////////////////////
		// Per-vertex input and output blocks. Depending on
		// the shader stage, these may be declared as arrays.
		uint32_t m_perVertexIn  = 0;
		uint32_t m_perVertexOut = 0;

		///////////////////////////////////////////////////////
		// Resource slot description for the shader.
		std::vector<vlt::VltResourceSlot> m_resourceSlots;
		////////////////////////////////////////////
		// Inter-stage shader interface slots. Also
		// covers vertex input and fragment output.
		vlt::VltInterfaceSlots m_interfaceSlots;
		//////////////////////////////////////////////////
		// Immediate constant buffer. If defined, this is
		// an array of four-component uint32 vectors.
		uint32_t                m_immConstBuf = 0;
		vlt::VltShaderConstData m_immConstData;

		///////////////////////////////////
		// Shader-specific data structures
		GcnCompilerVsPart m_vs;
		GcnCompilerPsPart m_ps;
		GcnCompilerCsPart m_cs;
	};

}  // namespace sce::gcn