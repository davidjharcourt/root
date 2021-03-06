// @(#)root/xml:$Id: d90d66e8fd2aa9daa4b05bcba9166aee1e2b2e7f $
// Author: Sergey Linev  10.05.2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TBufferXML
#define ROOT_TBufferXML

#include "TBuffer.h"
#include "TXMLSetup.h"
#include "TXMLEngine.h"
#include "TString.h"
#include "TObjArray.h"
#include "TArrayC.h"
#include "TClonesArray.h"

#include <string>

class TExMap;
class TVirtualStreamerInfo;
class TStreamerInfo;
class TStreamerElement;
class TObjArray;
class TMemberStreamer;
class TXMLFile;
class TXMLStackObj;

class TBufferXML : public TBuffer, public TXMLSetup {

   friend class TKeyXML;

public:
   TBufferXML(TBuffer::EMode mode);
   TBufferXML(TBuffer::EMode mode, TXMLFile *file);
   virtual ~TBufferXML();

   static TString ConvertToXML(const TObject *obj, Bool_t GenericLayout = kFALSE, Bool_t UseNamespaces = kFALSE);
   static TString
   ConvertToXML(const void *obj, const TClass *cl, Bool_t GenericLayout = kFALSE, Bool_t UseNamespaces = kFALSE);

   template <class T>
   static TString ToXML(const T *obj, Bool_t GenericLayout = kFALSE, Bool_t UseNamespaces = kFALSE)
   {
      return ConvertToXML(obj, TBuffer::GetClass(typeid(T)), GenericLayout, UseNamespaces);
   }

   static TObject *ConvertFromXML(const char *str, Bool_t GenericLayout = kFALSE, Bool_t UseNamespaces = kFALSE);
   static void *ConvertFromXMLAny(const char *str, TClass **cl = nullptr, Bool_t GenericLayout = kFALSE,
                                  Bool_t UseNamespaces = kFALSE);

   template <class T>
   static Bool_t FromXML(T *&obj, const char *xml, Bool_t GenericLayout = kFALSE, Bool_t UseNamespaces = kFALSE)
   {
      if (obj)
         return kFALSE;
      obj = (T *)ConvertFromXMLChecked(xml, TBuffer::GetClass(typeid(T)), GenericLayout, UseNamespaces);
      return obj != nullptr;
   }

   Int_t GetIOVersion() const { return fIOVersion; }
   void SetIOVersion(Int_t v) { fIOVersion = v; }

   // suppress class writing/reading

   virtual TClass *ReadClass(const TClass *cl = nullptr, UInt_t *objTag = nullptr);
   virtual void WriteClass(const TClass *cl);

   // redefined virtual functions of TBuffer

   virtual Int_t CheckByteCount(UInt_t startpos, UInt_t bcnt, const TClass *clss);    // SL
   virtual Int_t CheckByteCount(UInt_t startpos, UInt_t bcnt, const char *classname); // SL
   virtual void SetByteCount(UInt_t cntpos, Bool_t packInVersion = kFALSE);           // SL

   virtual void SkipVersion(const TClass *cl = nullptr);
   virtual Version_t ReadVersion(UInt_t *start = nullptr, UInt_t *bcnt = nullptr, const TClass *cl = nullptr); // SL
   virtual Version_t ReadVersionNoCheckSum(UInt_t *, UInt_t *);
   virtual UInt_t WriteVersion(const TClass *cl, Bool_t useBcnt = kFALSE); // SL

   virtual void *ReadObjectAny(const TClass *clCast);
   virtual void SkipObjectAny();

   virtual void IncrementLevel(TVirtualStreamerInfo *);
   virtual void SetStreamerElementNumber(TStreamerElement *elem, Int_t comp_type);
   virtual void DecrementLevel(TVirtualStreamerInfo *);

   virtual void ClassBegin(const TClass *, Version_t = -1);
   virtual void ClassEnd(const TClass *);
   virtual void ClassMember(const char *name, const char *typeName = nullptr, Int_t arrsize1 = -1, Int_t arrsize2 = -1);

   virtual void ReadFloat16(Float_t *f, TStreamerElement *ele = nullptr);
   virtual void WriteFloat16(Float_t *f, TStreamerElement *ele = nullptr);
   virtual void ReadDouble32(Double_t *d, TStreamerElement *ele = nullptr);
   virtual void WriteDouble32(Double_t *d, TStreamerElement *ele = nullptr);
   virtual void ReadWithFactor(Float_t *ptr, Double_t factor, Double_t minvalue);
   virtual void ReadWithNbits(Float_t *ptr, Int_t nbits);
   virtual void ReadWithFactor(Double_t *ptr, Double_t factor, Double_t minvalue);
   virtual void ReadWithNbits(Double_t *ptr, Int_t nbits);

   virtual Int_t ReadArray(Bool_t *&b);
   virtual Int_t ReadArray(Char_t *&c);
   virtual Int_t ReadArray(UChar_t *&c);
   virtual Int_t ReadArray(Short_t *&h);
   virtual Int_t ReadArray(UShort_t *&h);
   virtual Int_t ReadArray(Int_t *&i);
   virtual Int_t ReadArray(UInt_t *&i);
   virtual Int_t ReadArray(Long_t *&l);
   virtual Int_t ReadArray(ULong_t *&l);
   virtual Int_t ReadArray(Long64_t *&l);
   virtual Int_t ReadArray(ULong64_t *&l);
   virtual Int_t ReadArray(Float_t *&f);
   virtual Int_t ReadArray(Double_t *&d);
   virtual Int_t ReadArrayFloat16(Float_t *&f, TStreamerElement *ele = nullptr);
   virtual Int_t ReadArrayDouble32(Double_t *&d, TStreamerElement *ele = nullptr);

   virtual Int_t ReadStaticArray(Bool_t *b);
   virtual Int_t ReadStaticArray(Char_t *c);
   virtual Int_t ReadStaticArray(UChar_t *c);
   virtual Int_t ReadStaticArray(Short_t *h);
   virtual Int_t ReadStaticArray(UShort_t *h);
   virtual Int_t ReadStaticArray(Int_t *i);
   virtual Int_t ReadStaticArray(UInt_t *i);
   virtual Int_t ReadStaticArray(Long_t *l);
   virtual Int_t ReadStaticArray(ULong_t *l);
   virtual Int_t ReadStaticArray(Long64_t *l);
   virtual Int_t ReadStaticArray(ULong64_t *l);
   virtual Int_t ReadStaticArray(Float_t *f);
   virtual Int_t ReadStaticArray(Double_t *d);
   virtual Int_t ReadStaticArrayFloat16(Float_t *f, TStreamerElement *ele = nullptr);
   virtual Int_t ReadStaticArrayDouble32(Double_t *d, TStreamerElement *ele = nullptr);

   virtual void ReadFastArray(Bool_t *b, Int_t n);
   virtual void ReadFastArray(Char_t *c, Int_t n);
   virtual void ReadFastArray(UChar_t *c, Int_t n);
   virtual void ReadFastArray(Short_t *h, Int_t n);
   virtual void ReadFastArray(UShort_t *h, Int_t n);
   virtual void ReadFastArray(Int_t *i, Int_t n);
   virtual void ReadFastArray(UInt_t *i, Int_t n);
   virtual void ReadFastArray(Long_t *l, Int_t n);
   virtual void ReadFastArray(ULong_t *l, Int_t n);
   virtual void ReadFastArray(Long64_t *l, Int_t n);
   virtual void ReadFastArray(ULong64_t *l, Int_t n);
   virtual void ReadFastArray(Float_t *f, Int_t n);
   virtual void ReadFastArray(Double_t *d, Int_t n);
   virtual void ReadFastArrayString(Char_t *c, Int_t n);
   virtual void ReadFastArrayFloat16(Float_t *f, Int_t n, TStreamerElement *ele = nullptr);
   virtual void ReadFastArrayDouble32(Double_t *d, Int_t n, TStreamerElement *ele = nullptr);
   virtual void ReadFastArrayWithFactor(Float_t *ptr, Int_t n, Double_t factor, Double_t minvalue);
   virtual void ReadFastArrayWithNbits(Float_t *ptr, Int_t n, Int_t nbits);
   virtual void ReadFastArrayWithFactor(Double_t *ptr, Int_t n, Double_t factor, Double_t minvalue);
   virtual void ReadFastArrayWithNbits(Double_t *ptr, Int_t n, Int_t nbits);

   virtual void WriteArray(const Bool_t *b, Int_t n);
   virtual void WriteArray(const Char_t *c, Int_t n);
   virtual void WriteArray(const UChar_t *c, Int_t n);
   virtual void WriteArray(const Short_t *h, Int_t n);
   virtual void WriteArray(const UShort_t *h, Int_t n);
   virtual void WriteArray(const Int_t *i, Int_t n);
   virtual void WriteArray(const UInt_t *i, Int_t n);
   virtual void WriteArray(const Long_t *l, Int_t n);
   virtual void WriteArray(const ULong_t *l, Int_t n);
   virtual void WriteArray(const Long64_t *l, Int_t n);
   virtual void WriteArray(const ULong64_t *l, Int_t n);
   virtual void WriteArray(const Float_t *f, Int_t n);
   virtual void WriteArray(const Double_t *d, Int_t n);
   virtual void WriteArrayFloat16(const Float_t *f, Int_t n, TStreamerElement *ele = nullptr);
   virtual void WriteArrayDouble32(const Double_t *d, Int_t n, TStreamerElement *ele = nullptr);
   virtual void ReadFastArray(void *start, const TClass *cl, Int_t n = 1, TMemberStreamer *s = nullptr,
                              const TClass *onFileClass = nullptr);
   virtual void ReadFastArray(void **startp, const TClass *cl, Int_t n = 1, Bool_t isPreAlloc = kFALSE,
                              TMemberStreamer *s = nullptr, const TClass *onFileClass = nullptr);

   virtual void WriteFastArray(const Bool_t *b, Int_t n);
   virtual void WriteFastArray(const Char_t *c, Int_t n);
   virtual void WriteFastArray(const UChar_t *c, Int_t n);
   virtual void WriteFastArray(const Short_t *h, Int_t n);
   virtual void WriteFastArray(const UShort_t *h, Int_t n);
   virtual void WriteFastArray(const Int_t *i, Int_t n);
   virtual void WriteFastArray(const UInt_t *i, Int_t n);
   virtual void WriteFastArray(const Long_t *l, Int_t n);
   virtual void WriteFastArray(const ULong_t *l, Int_t n);
   virtual void WriteFastArray(const Long64_t *l, Int_t n);
   virtual void WriteFastArray(const ULong64_t *l, Int_t n);
   virtual void WriteFastArray(const Float_t *f, Int_t n);
   virtual void WriteFastArray(const Double_t *d, Int_t n);
   virtual void WriteFastArrayString(const Char_t *c, Int_t n);
   virtual void WriteFastArrayFloat16(const Float_t *d, Int_t n, TStreamerElement *ele = nullptr);
   virtual void WriteFastArrayDouble32(const Double_t *d, Int_t n, TStreamerElement *ele = nullptr);
   virtual void WriteFastArray(void *start, const TClass *cl, Int_t n = 1, TMemberStreamer *s = nullptr);
   virtual Int_t WriteFastArray(void **startp, const TClass *cl, Int_t n = 1, Bool_t isPreAlloc = kFALSE,
                                TMemberStreamer *s = nullptr);

   virtual void StreamObject(void *obj, const std::type_info &typeinfo, const TClass *onFileClass = nullptr);
   virtual void StreamObject(void *obj, const char *className, const TClass *onFileClass = nullptr);
   virtual void StreamObject(void *obj, const TClass *cl, const TClass *onFileClass = nullptr);
   virtual void StreamObject(TObject *obj);

   virtual void ReadBool(Bool_t &b);
   virtual void ReadChar(Char_t &c);
   virtual void ReadUChar(UChar_t &c);
   virtual void ReadShort(Short_t &s);
   virtual void ReadUShort(UShort_t &s);
   virtual void ReadInt(Int_t &i);
   virtual void ReadUInt(UInt_t &i);
   virtual void ReadLong(Long_t &l);
   virtual void ReadULong(ULong_t &l);
   virtual void ReadLong64(Long64_t &l);
   virtual void ReadULong64(ULong64_t &l);
   virtual void ReadFloat(Float_t &f);
   virtual void ReadDouble(Double_t &d);
   virtual void ReadCharP(Char_t *c);
   virtual void ReadTString(TString &s);
   virtual void ReadStdString(std::string *s);
   using TBuffer::ReadStdString;
   virtual void ReadCharStar(char *&s);

   virtual void WriteBool(Bool_t b);
   virtual void WriteChar(Char_t c);
   virtual void WriteUChar(UChar_t c);
   virtual void WriteShort(Short_t s);
   virtual void WriteUShort(UShort_t s);
   virtual void WriteInt(Int_t i);
   virtual void WriteUInt(UInt_t i);
   virtual void WriteLong(Long_t l);
   virtual void WriteULong(ULong_t l);
   virtual void WriteLong64(Long64_t l);
   virtual void WriteULong64(ULong64_t l);
   virtual void WriteFloat(Float_t f);
   virtual void WriteDouble(Double_t d);
   virtual void WriteCharP(const Char_t *c);
   virtual void WriteTString(const TString &s);
   virtual void WriteStdString(const std::string *s);
   using TBuffer::WriteStdString;
   virtual void WriteCharStar(char *s);

   virtual Int_t ApplySequence(const TStreamerInfoActions::TActionSequence &sequence, void *object);
   virtual Int_t ApplySequenceVecPtr(const TStreamerInfoActions::TActionSequence &sequence, void *start_collection,
                                     void *end_collection);
   virtual Int_t
   ApplySequence(const TStreamerInfoActions::TActionSequence &sequence, void *start_collection, void *end_collection);

   // end of redefined virtual functions

   // abstract TBuffer methods, probably dedicated for TBufferText

   virtual Bool_t CheckObject(const TObject *obj);

   virtual Bool_t CheckObject(const void *ptr, const TClass *cl);

   virtual Int_t ReadBuf(void * /*buf*/, Int_t /*max*/)
   {
      Error("ReadBuf", "useless");
      return 0;
   }

   virtual void WriteBuf(const void * /*buf*/, Int_t /*max*/) { Error("WriteBuf", "useless"); }

   virtual char *ReadString(char * /*s*/, Int_t /*max*/)
   {
      Error("ReadString", "useless");
      return 0;
   }

   virtual void WriteString(const char * /*s*/) { Error("WriteString", "useless"); }

   virtual Int_t GetVersionOwner() const
   {
      Error("GetVersionOwner", "useless");
      return 0;
   }
   virtual Int_t GetMapCount() const
   {
      Error("GetMapCount", "useless");
      return 0;
   }
   virtual void GetMappedObject(UInt_t /*tag*/, void *& /*ptr*/, TClass *& /*ClassPtr*/) const
   {
      Error("GetMappedObject", "useless");
   }
   virtual void MapObject(const TObject * /*obj*/, UInt_t /*offset*/ = 1) { Error("MapObject", "useless"); }
   virtual void MapObject(const void * /*obj*/, const TClass * /*cl*/, UInt_t /*offset*/ = 1)
   {
      Error("MapObject", "useless");
   }
   virtual void Reset() { Error("Reset", "useless"); }
   virtual void InitMap() { Error("InitMap", "useless"); }
   virtual void ResetMap() { Error("ResetMap", "useless"); }
   virtual void SetReadParam(Int_t /*mapsize*/) { Error("SetReadParam", "useless"); }
   virtual void SetWriteParam(Int_t /*mapsize*/) { Error("SetWriteParam", "useless"); }

   virtual Version_t ReadVersionForMemberWise(const TClass * /*cl*/ = nullptr)
   {
      Error("ReadVersionForMemberWise", "useless");
      return 0;
   }
   virtual UInt_t WriteVersionMemberWise(const TClass * /*cl*/, Bool_t /*useBcnt*/ = kFALSE)
   {
      Error("WriteVersionMemberWise", "useless");
      return 0;
   }

   virtual TVirtualStreamerInfo *GetInfo();

   virtual TObject *ReadObject(const TClass * /*cl*/)
   {
      Error("ReadObject", "useless");
      return 0;
   }

   virtual UShort_t GetPidOffset() const { return fPidOffset; }
   virtual void SetPidOffset(UShort_t offset) { fPidOffset = offset; }
   virtual Int_t GetBufferDisplacement() const
   {
      Error("GetBufferDisplacement", "useless");
      return 0;
   }
   virtual void SetBufferDisplacement() { Error("SetBufferDisplacement", "useless"); }
   virtual void SetBufferDisplacement(Int_t /*skipped*/) { Error("SetBufferDisplacement", "useless"); }

   virtual TProcessID *GetLastProcessID(TRefTable * /*reftable*/) const
   {
      Error("GetLastProcessID", "useless");
      return 0;
   }
   virtual UInt_t GetTRefExecId();
   virtual TProcessID *ReadProcessID(UShort_t pidf);
   virtual UShort_t WriteProcessID(TProcessID *pid);

   // Utilities for TStreamerInfo
   virtual void ForceWriteInfo(TVirtualStreamerInfo *info, Bool_t force);
   virtual void ForceWriteInfoClones(TClonesArray *a);
   virtual Int_t ReadClones(TClonesArray *a, Int_t nobjects, Version_t objvers);
   virtual Int_t WriteClones(TClonesArray *a, Int_t nobjects);

   // Utilities for TClass
   virtual Int_t ReadClassEmulated(const TClass * /*cl*/, void * /*object*/, const TClass * /*onfile_class*/ = nullptr)
   {
      Error("ReadClassEmulated", "useless");
      return 0;
   }

   virtual Int_t ReadClassBuffer(const TClass * /*cl*/, void * /*pointer*/, const TClass * /*onfile_class*/ = nullptr);
   virtual Int_t ReadClassBuffer(const TClass * /*cl*/, void * /*pointer*/, Int_t /*version*/, UInt_t /*start*/,
                                 UInt_t /*count*/, const TClass * /*onfile_class*/ = nullptr);

   virtual Int_t WriteClassBuffer(const TClass *cl, void *pointer);

   virtual void TagStreamerInfo(TVirtualStreamerInfo * /*info*/);

   virtual void WriteObject(const TObject *obj, Bool_t cacheReuse = kTRUE);
   virtual Int_t WriteObjectAny(const void *obj, const TClass *ptrClass, Bool_t cacheReuse = kTRUE);

   using TBuffer::WriteObject;

   // end of abstract TBuffer methods, probably dedicated for TBufferText

   static void SetFloatFormat(const char *fmt = "%e");
   static const char *GetFloatFormat();

protected:
   TBufferXML();

   // redefined protected virtual functions

   virtual void WriteObjectClass(const void *actualObjStart, const TClass *actualClass, Bool_t cacheReuse);

   // end redefined protected virtual functions

   static void *ConvertFromXMLChecked(const char *xml, const TClass *expectedClass, Bool_t GenericLayout = kFALSE,
                                      Bool_t UseNamespaces = kFALSE);

   TXMLFile *XmlFile();

   Int_t GetCompressionAlgorithm() const;
   Int_t GetCompressionLevel() const;
   Int_t GetCompressionSettings() const;
   void SetCompressionAlgorithm(Int_t algorithm = 0);
   void SetCompressionLevel(Int_t level = 1);
   void SetCompressionSettings(Int_t settings = 1);
   void SetXML(TXMLEngine *xml) { fXML = xml; }

   void XmlWriteBlock(XMLNodePointer_t node);
   XMLNodePointer_t XmlWriteAny(const void *obj, const TClass *cl);

   void XmlReadBlock(XMLNodePointer_t node);
   void *XmlReadAny(XMLNodePointer_t node, void *obj, TClass **cl);

   TXMLStackObj *PushStack(XMLNodePointer_t current, Bool_t simple = kFALSE);
   TXMLStackObj *PopStack();
   void ShiftStack(const char *info = nullptr);

   XMLNodePointer_t StackNode();
   TXMLStackObj *Stack(Int_t depth = 0);

   void WorkWithClass(TStreamerInfo *info, const TClass *cl = nullptr);
   void WorkWithElement(TStreamerElement *elem, Int_t comp_type);
   Bool_t VerifyNode(XMLNodePointer_t node, const char *name, const char *errinfo = nullptr);
   Bool_t VerifyStackNode(const char *name, const char *errinfo = nullptr);

   Bool_t VerifyAttr(XMLNodePointer_t node, const char *name, const char *value, const char *errinfo = nullptr);
   Bool_t VerifyStackAttr(const char *name, const char *value, const char *errinfo = nullptr);

   Bool_t ProcessPointer(const void *ptr, XMLNodePointer_t node);
   void RegisterPointer(const void *ptr, XMLNodePointer_t node);
   Bool_t ExtractPointer(XMLNodePointer_t node, void *&ptr, TClass *&cl);
   void ExtractReference(XMLNodePointer_t node, const void *ptr, const TClass *cl);

   XMLNodePointer_t CreateItemNode(const char *name);
   Bool_t VerifyItemNode(const char *name, const char *errinfo = nullptr);

   void CreateElemNode(const TStreamerElement *elem);
   Bool_t VerifyElemNode(const TStreamerElement *elem);

   void PerformPreProcessing(const TStreamerElement *elem, XMLNodePointer_t elemnode);
   void PerformPostProcessing();

   XMLNodePointer_t XmlWriteBasic(Char_t value);
   XMLNodePointer_t XmlWriteBasic(Short_t value);
   XMLNodePointer_t XmlWriteBasic(Int_t value);
   XMLNodePointer_t XmlWriteBasic(Long_t value);
   XMLNodePointer_t XmlWriteBasic(Long64_t value);
   XMLNodePointer_t XmlWriteBasic(Float_t value);
   XMLNodePointer_t XmlWriteBasic(Double_t value);
   XMLNodePointer_t XmlWriteBasic(Bool_t value);
   XMLNodePointer_t XmlWriteBasic(UChar_t value);
   XMLNodePointer_t XmlWriteBasic(UShort_t value);
   XMLNodePointer_t XmlWriteBasic(UInt_t value);
   XMLNodePointer_t XmlWriteBasic(ULong_t value);
   XMLNodePointer_t XmlWriteBasic(ULong64_t value);
   XMLNodePointer_t XmlWriteValue(const char *value, const char *name);

   void XmlReadBasic(Char_t &value);
   void XmlReadBasic(Short_t &value);
   void XmlReadBasic(Int_t &value);
   void XmlReadBasic(Long_t &value);
   void XmlReadBasic(Long64_t &value);
   void XmlReadBasic(Float_t &value);
   void XmlReadBasic(Double_t &value);
   void XmlReadBasic(Bool_t &value);
   void XmlReadBasic(UChar_t &value);
   void XmlReadBasic(UShort_t &value);
   void XmlReadBasic(UInt_t &value);
   void XmlReadBasic(ULong_t &value);
   void XmlReadBasic(ULong64_t &value);
   const char *XmlReadValue(const char *name);

   template <typename T>
   R__ALWAYS_INLINE void XmlReadArrayContent(T *arr, Int_t arrsize);

   template <typename T>
   R__ALWAYS_INLINE Int_t XmlReadArray(T *&arr, bool is_static = false);

   template <typename T>
   R__ALWAYS_INLINE void XmlReadFastArray(T *arr, Int_t n);

   template <typename T>
   R__ALWAYS_INLINE void XmlWriteArrayContent(const T *arr, Int_t arrsize);

   template <typename T>
   R__ALWAYS_INLINE void XmlWriteArray(const T *arr, Int_t arrsize);

   template <typename T>
   R__ALWAYS_INLINE void XmlWriteFastArray(const T *arr, Int_t n);

   XMLNodePointer_t XmlWriteObject(const void *obj, const TClass *objClass, Bool_t cacheReuse);
   void *XmlReadObject(void *obj, TClass **cl = nullptr);

   void BeforeIOoperation();
   void CheckVersionBuf();

   TXMLEngine *fXML; //!

   TObjArray fStack; //!

   Version_t fVersionBuf; //!

   TExMap *fObjMap;     //!
   TObjArray *fIdArray; //!

   TString fValueBuf; //!

   Int_t fErrorFlag; //!

   Bool_t fCanUseCompact; ///<!   Flag indicate that basic type (like Int_t) can be placed in the same tag
   Bool_t fExpectedChain; ///<!   Flag to resolve situation when several elements of same basic type stored as FastArray
   TClass *fExpectedBaseClass; ///<!   Pointer to class, which should be stored as parent of current
   Int_t fCompressLevel;       ///<!   Compression level and algorithm
   Int_t fIOVersion;           ///<!   Indicates format of ROOT xml file

   UShort_t fPidOffset; ///<! PID offset

   static std::string
      fgFloatFmt; ///<!   Printf argument for floats and doubles, either "%f" or "%e" or "%10f" and so on

   ClassDef(TBufferXML, 0); // a specialized TBuffer to read/write to XML files
};

//______________________________________________________________________________
inline Int_t TBufferXML::GetCompressionAlgorithm() const
{
   return (fCompressLevel < 0) ? -1 : fCompressLevel / 100;
}

//______________________________________________________________________________
inline Int_t TBufferXML::GetCompressionLevel() const
{
   return (fCompressLevel < 0) ? -1 : fCompressLevel % 100;
}

//______________________________________________________________________________
inline Int_t TBufferXML::GetCompressionSettings() const
{
   return (fCompressLevel < 0) ? -1 : fCompressLevel;
}

#endif
