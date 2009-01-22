#ifndef _TextSection_h_
#define _TextSection_h_

#include <Base.h>
#include <RawSection.h>
#include <Vector.h>

class ElfFile;
class Instruction;
class BasicBlock;
class Function;
class BinaryInputFile;
class BinaryOutputFile;
class Disassembler;
class Symbol;
class TextSection;

class TextObject : public Base {
protected:
    TextSection* textSection;
    uint32_t index;
    uint64_t address;

public:
    TextObject(ElfClassTypes typ, TextSection* text, uint32_t idx, uint64_t addr, uint32_t sz);
    ~TextObject() {}

    uint32_t getIndex() { return index; }
    uint64_t getAddress() { return address; }
    bool inRange(uint64_t addr);
    char* charStream();
    bool isFunction();

    virtual uint32_t getNumberOfInstructions() { __SHOULD_NOT_ARRIVE; }
    TextSection* getTextSection() { return textSection; }

    virtual void dump(BinaryOutputFile* binaryOutputFile, uint32_t offset) { __SHOULD_NOT_ARRIVE; }
    virtual char* getName() { __SHOULD_NOT_ARRIVE; }
    virtual uint32_t digest() { __SHOULD_NOT_ARRIVE; }

    virtual void print() { __SHOULD_NOT_ARRIVE; }
};

class TextUnknown : public TextObject {
protected:
    Symbol* symbol;
public:
    TextUnknown(TextSection* text, uint32_t idx, Symbol* sym, uint64_t addr, uint32_t sz);
    ~TextUnknown() {}

    uint32_t getNumberOfInstructions() { __SHOULD_NOT_ARRIVE; }

    void dump(BinaryOutputFile* binaryOutputFile, uint32_t offset);
    char* getName();
    uint32_t digest();
};

class FreeText : public TextObject {
protected:
    Vector<Instruction*> instructions;

public:
    FreeText(TextSection* text, uint32_t idx, uint64_t addr, uint32_t sz);
    ~FreeText();

    void dump(BinaryOutputFile* binaryOutputFile, uint32_t offset);
    char* getName() { return NULL; }
    uint32_t digest();

    Instruction* getInstruction(uint32_t idx) { return instructions[idx]; }
    uint32_t getNumberOfInstructions() { return instructions.size(); }

    void print();
};

class TextSection : public RawSection {
protected:
    uint32_t index;
    Vector<TextObject*> sortedTextObjects;

    Disassembler* disassembler;
    ByteSources source;

public:
    TextSection(char* filePtr, uint64_t size, uint16_t scnIdx, uint32_t idx, ElfFile* elf, ByteSources src);
    ~TextSection();

    void printInstructions();
    Vector<Symbol*> discoverTextObjects();

    ByteSources getByteSource();
    ByteSources setByteSource(ByteSources src);

    uint32_t readNoFile();
    uint32_t getIndex() { return index; }
    uint32_t disassemble();
    uint32_t printDisassembledCode(bool instructionDetail);
    uint32_t read(BinaryInputFile* b);
    uint32_t disassemble(BinaryInputFile* b);

    uint64_t findInstrumentationPoint(uint32_t size, InstLocations loc);
    Disassembler* getDisassembler() { return disassembler; }

    bool verify();
    const char* briefName() { return "TextSection"; }
    void dump (BinaryOutputFile* binaryOutputFile, uint32_t offset);

    Instruction* getInstructionAtAddress(uint64_t addr);

    uint64_t getAddress();
    bool inRange(uint64_t addr);

    uint32_t getNumberOfTextObjects() { return sortedTextObjects.size(); }
    TextObject* getTextObject(uint32_t idx) { return sortedTextObjects[idx]; }

    uint32_t getNumberOfBasicBlocks();
    uint32_t getNumberOfMemoryOps();
    uint32_t getNumberOfFloatOps();
    uint32_t getNumberOfInstructions();

    Vector<Instruction*>* swapInstructions(uint64_t addr, Vector<Instruction*>* replacements);

    void printLoops();
    uint32_t buildLoops();
};


#endif /* _TextSection_h_ */

