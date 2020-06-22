//
// Generated file, do not edit! Created by nedtool 5.5 from application/msg/CRL_Piece.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "CRL_Piece_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace veins {

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(CRL_Piece)

CRL_Piece::CRL_Piece(const char *name, short kind) : ::veins::BaseFrame1609_4(name,kind)
{
    this->sender = -1;
    this->CRLversion = 0;
    this->Timestamp = 0;
    this->SeqNo = 0;
    this->CAid = 0;
    this->pad = 0;
}

CRL_Piece::CRL_Piece(const CRL_Piece& other) : ::veins::BaseFrame1609_4(other)
{
    copy(other);
}

CRL_Piece::~CRL_Piece()
{
}

CRL_Piece& CRL_Piece::operator=(const CRL_Piece& other)
{
    if (this==&other) return *this;
    ::veins::BaseFrame1609_4::operator=(other);
    copy(other);
    return *this;
}

void CRL_Piece::copy(const CRL_Piece& other)
{
    this->sender = other.sender;
    this->CRLversion = other.CRLversion;
    this->Timestamp = other.Timestamp;
    this->SeqNo = other.SeqNo;
    this->CAid = other.CAid;
    this->pad = other.pad;
    this->payload = other.payload;
}

void CRL_Piece::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::veins::BaseFrame1609_4::parsimPack(b);
    doParsimPacking(b,this->sender);
    doParsimPacking(b,this->CRLversion);
    doParsimPacking(b,this->Timestamp);
    doParsimPacking(b,this->SeqNo);
    doParsimPacking(b,this->CAid);
    doParsimPacking(b,this->pad);
    doParsimPacking(b,this->payload);
}

void CRL_Piece::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::veins::BaseFrame1609_4::parsimUnpack(b);
    doParsimUnpacking(b,this->sender);
    doParsimUnpacking(b,this->CRLversion);
    doParsimUnpacking(b,this->Timestamp);
    doParsimUnpacking(b,this->SeqNo);
    doParsimUnpacking(b,this->CAid);
    doParsimUnpacking(b,this->pad);
    doParsimUnpacking(b,this->payload);
}

LAddress::L2Type& CRL_Piece::getSender()
{
    return this->sender;
}

void CRL_Piece::setSender(const LAddress::L2Type& sender)
{
    this->sender = sender;
}

int CRL_Piece::getCRLversion() const
{
    return this->CRLversion;
}

void CRL_Piece::setCRLversion(int CRLversion)
{
    this->CRLversion = CRLversion;
}

int CRL_Piece::getTimestamp() const
{
    return this->Timestamp;
}

void CRL_Piece::setTimestamp(int Timestamp)
{
    this->Timestamp = Timestamp;
}

int CRL_Piece::getSeqNo() const
{
    return this->SeqNo;
}

void CRL_Piece::setSeqNo(int SeqNo)
{
    this->SeqNo = SeqNo;
}

int CRL_Piece::getCAid() const
{
    return this->CAid;
}

void CRL_Piece::setCAid(int CAid)
{
    this->CAid = CAid;
}

int CRL_Piece::getPad() const
{
    return this->pad;
}

void CRL_Piece::setPad(int pad)
{
    this->pad = pad;
}

const char * CRL_Piece::getPayload() const
{
    return this->payload.c_str();
}

void CRL_Piece::setPayload(const char * payload)
{
    this->payload = payload;
}

class CRL_PieceDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CRL_PieceDescriptor();
    virtual ~CRL_PieceDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CRL_PieceDescriptor)

CRL_PieceDescriptor::CRL_PieceDescriptor() : omnetpp::cClassDescriptor("veins::CRL_Piece", "veins::BaseFrame1609_4")
{
    propertynames = nullptr;
}

CRL_PieceDescriptor::~CRL_PieceDescriptor()
{
    delete[] propertynames;
}

bool CRL_PieceDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CRL_Piece *>(obj)!=nullptr;
}

const char **CRL_PieceDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CRL_PieceDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CRL_PieceDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int CRL_PieceDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *CRL_PieceDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sender",
        "CRLversion",
        "Timestamp",
        "SeqNo",
        "CAid",
        "pad",
        "payload",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int CRL_PieceDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sender")==0) return base+0;
    if (fieldName[0]=='C' && strcmp(fieldName, "CRLversion")==0) return base+1;
    if (fieldName[0]=='T' && strcmp(fieldName, "Timestamp")==0) return base+2;
    if (fieldName[0]=='S' && strcmp(fieldName, "SeqNo")==0) return base+3;
    if (fieldName[0]=='C' && strcmp(fieldName, "CAid")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "pad")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CRL_PieceDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "LAddress::L2Type",
        "int",
        "int",
        "int",
        "int",
        "int",
        "string",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **CRL_PieceDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CRL_PieceDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CRL_PieceDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CRL_PieceDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CRL_PieceDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSender(); return out.str();}
        case 1: return long2string(pp->getCRLversion());
        case 2: return long2string(pp->getTimestamp());
        case 3: return long2string(pp->getSeqNo());
        case 4: return long2string(pp->getCAid());
        case 5: return long2string(pp->getPad());
        case 6: return oppstring2string(pp->getPayload());
        default: return "";
    }
}

bool CRL_PieceDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        case 1: pp->setCRLversion(string2long(value)); return true;
        case 2: pp->setTimestamp(string2long(value)); return true;
        case 3: pp->setSeqNo(string2long(value)); return true;
        case 4: pp->setCAid(string2long(value)); return true;
        case 5: pp->setPad(string2long(value)); return true;
        case 6: pp->setPayload((value)); return true;
        default: return false;
    }
}

const char *CRL_PieceDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(LAddress::L2Type));
        default: return nullptr;
    };
}

void *CRL_PieceDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CRL_Piece *pp = (CRL_Piece *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSender()); break;
        default: return nullptr;
    }
}

} // namespace veins

