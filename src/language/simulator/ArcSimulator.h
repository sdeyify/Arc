#pragma once
#include "StackValueKind.h"
#include "InstructionResult.h"
#include "RuntimeValue.h"
#include "HeapObject.h"
#include "SimulationEnvironment.h"
#include "Debugger.h"
#include "../parser/Parser.h"
#include "../util/imports.h"

namespace Arc
{
    class ArcSimulator
    {
    public:
        ArcSimulator(
            const IRProgram& ir, const SimulationEnvironment& environment
        ) : ir(ir), environment(environment), debugger(Debugger(*this)) {}
    public:
        friend class Debugger;
    private:
        const IRProgram& ir;

        Debugger debugger;

        bool displayedBreakpointMessage = false;

        const SimulationEnvironment& environment;

        std::stack<size_t> stackLocation;
        std::map<size_t, RuntimeValue> locals;
        std::deque<RuntimeValue> stack;
        std::vector<HeapObject> heap;
        std::vector<ArrayData> arrayData;
        std::vector<RuntimeValue> pointers;

        std::map<size_t, size_t> labels;
    public:
        int simulate();
    private:
        std::optional<int> simulateIR(const size_t locationPtr);
        InstructionResult simulateInstruction(const Instruction &instruction);

        std::string inspectValue(const RuntimeValue& value);
        void evaluateCode(const std::string& src);

        std::string replReadUserInput() const;

        void loadLabels();

        void addStackLocation(const size_t locationPtr);
        void updateStackLocation(const size_t locationPtr);
        size_t removeStackLocation();
        std::stack<size_t> getStackLocation();
        size_t& getTopStackLocation();

        RuntimeValue popStackValue();

        template<typename T = std::intmax_t> requires runtime_value_v<T>
        constexpr auto interpretStackValue(const RuntimeValue& value) {
            if constexpr (std::is_same_v<T, std::monostate>()) {
                return std::monostate();
            } else if constexpr (float_runtime_value_v<T>) {
                return static_cast<double>(std::visit<T>([](const auto&& value) { return static_cast<double>(value); }, value.value));
            } else {
                return static_cast<T>(std::visit<T>([](const auto&& value) { return static_cast<intmax_t>(value); }, value.value));
            }
        }

        template<typename T = std::intmax_t> requires runtime_value_v<T>
        constexpr T getStackValue() {
            const Arc::RuntimeValue value = this->popStackValue();
            
            if (
                value == Arc::IRBitsizeKind::NativeF16 ||
                value == Arc::IRBitsizeKind::NativeBF16 ||
                value == Arc::IRBitsizeKind::NativeF32 ||
                value == Arc::IRBitsizeKind::NativeF64 ||
            ) {
                return this->interpretStackValue<double>(this->popStackValue());
            } else {
                return this->interpretStackValue<T>(this->popStackValue());
            }

        }

        RuntimeValue castBitValue(const int64_t value, const IRBitsizeKind& kind, const StackValueKind& valueKind);

        RuntimeValue interpretRuntimeValue(const RuntimeValue& value, const std::optional<IRBitsizeKind>& bitsize, const StackValueKind& valueKind);

        void pushStringifiedBitvalue(const std::string& rawRepresentation, const IRBitsizeKind& bitsize, const StackValueKind& valueKind);

        void parseArrayLength();
        void parseBinaryOperation(const IRNodes::BinaryOperation& instruction);
        void parseBitcast(const Arc::IRBitsizeKind& endBitsize);

        void parseBreakpoint(const Arc::IRNodes::Breakpoint& instruction);
        void parseReplBreakpoint();
        void parseDebugBreakpoint();

        void parseBranchIf(const size_t labelId);
        void parseBubble(const size_t stackAddress);
        void parseCall(const size_t functionId);
        void parseCallIndirect();
        void parseDeleteObject();
        void parseFreePointer();
        void parseFunctionPointerOf(const size_t functionId);
        void parseLoadArray(const std::optional<IRBitsizeKind>& bitsize);
        void parseLoadDirectivePointer(const Arc::IRNodes::LoadDirectivePointer& instruction);
        void parseLoadField(const Arc::IRNodes::LoadField& instruction);
        void parseLoadLocal(const IRNodes::LoadLocal& instruction);
        void parseLoadValue(const IRNodes::LoadValue& instruction);
        void parseLoadPointer(const std::optional<IRBitsizeKind>& bitsize);
        void parseNewArray(const IRBitsizeKind& bitsize);
        void parseNewObject(const size_t structId);
        void parseObjectCast(const size_t endStructId);
        void parsePointer(const IRBitsizeKind& bitsize);
        void parseStoreArray();
        void parseStoreField(const Arc::IRNodes::StoreField& instruction);
        void parseStoreLocal(const IRNodes::StoreLocal& instruction);
        void parseStorePointer(const IRNodes::StorePointer& instruction);
        void parseSwitch(const std::vector<size_t>& switchLabelIds);
        void parseUnaryOperation(const IRNodes::UnaryOperation& instruction);
    };
}