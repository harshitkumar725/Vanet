//
// Generated file, do not edit! Created by nedtool 5.5 from application/msg/Beacon.msg.
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
#include "Beacon_m.h"

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

Register_Class(Beacon)

Beacon::Beacon(const char *name, short kind) : ::veins::BaseFrame1609_4(name,kind)
{
    this->destAddr = -1;
    this->srcAddr = -1;
    this->positionX = 0;
    this->positionY = 0;
    this->speed = 0;
    this->myResult = 0;
    this->NeedCRL = false;
    this->CRL_Piece_No = 0;
    this->rangeS = 0;
    this->rangeE = 0;
    this->CRLchannel = 0;
}

Beacon::Beacon(const Beacon& other) : ::veins::BaseFrame1609_4(other)
{
    copy(other);
}

Beacon::~Beacon()
{
}

Beacon& Beacon::operator=(const Beacon& other)
{
    if (this==&other) return *this;
    ::veins::BaseFrame1609_4::operator=(other);
    copy(other);
    return *this;
}

void Beacon::copy(const Beacon& other)
{
    this->destAddr = other.destAddr;
    this->srcAddr = other.srcAddr;
    this->NodeName = other.NodeName;
    this->positionX = other.positionX;
    this->positionY = other.positionY;
    this->speed = other.speed;
    this->myResult = other.myResult;
    this->NeedCRL = other.NeedCRL;
    this->CRL_Piece_No = other.CRL_Piece_No;
    this->rangeS = other.rangeS;
    this->rangeE = other.rangeE;
    this->CRLchannel = other.CRLchannel;
}

void Beacon::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::veins::BaseFrame1609_4::parsimPack(b);
    doParsimPacking(b,this->destAddr);
    doParsimPacking(b,this->srcAddr);
    doParsimPacking(b,this->NodeName);
    doParsimPacking(b,this->positionX);
    doParsimPacking(b,this->positionY);
    doParsimPacking(b,this->speed);
    doParsimPacking(b,this->myResult);
    doParsimPacking(b,this->NeedCRL);
    doParsimPacking(b,this->CRL_Piece_No);
    doParsimPacking(b,this->rangeS);
    doParsimPacking(b,this->rangeE);
    doParsimPacking(b,this->CRLchannel);
}

void Beacon::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::veins::BaseFrame1609_4::parsimUnpack(b);
    doParsimUnpacking(b,this->destAddr);
    doParsimUnpacking(b,this->srcAddr);
    doParsimUnpacking(b,this->NodeName);
    doParsimUnpacking(b,this->positionX);
    doParsimUnpacking(b,this->positionY);
    doParsimUnpacking(b,this->speed);
    doParsimUnpacking(b,this->myResult);
    doParsimUnpacking(b,this->NeedCRL);
    doParsimUnpacking(b,this->CRL_Piece_No);
    doParsimUnpacking(b,this->rangeS);
    doParsimUnpacking(b,this->rangeE);
    doParsimUnpacking(b,this->CRLchannel);
}

LAddress::L2Type& Beacon::getDestAddr()
{
    return this->destAddr;
}

void Beacon::setDestAddr(const LAddress::L2Type& destAddr)
{
    this->destAddr = destAddr;
}

LAddress::L2Type& Beacon::getSrcAddr()
{
    return this->srcAddr;
}

void Beacon::setSrcAddr(const LAddress::L2Type& srcAddr)
{
    this->srcAddr = srcAddr;
}

const char * Beacon::getNodeName() const
{
    return this->NodeName.c_str();
}

void Beacon::setNodeName(const char * NodeName)
{
    this->NodeName = NodeName;
}

double Beacon::getPositionX() const
{
    return this->positionX;
}

void Beacon::setPositionX(double positionX)
{
    this->positionX = positionX;
}

double Beacon::getPositionY() const
{
    return this->positionY;
}

void Beacon::setPositionY(double positionY)
{
    this->positionY = positionY;
}

double Beacon::getSpeed() const
{
    return this->speed;
}

void Beacon::setSpeed(double speed)
{
    this->speed = speed;
}

int Beacon::getMyResult() const
{
    return this->myResult;
}

void Beacon::setMyResult(int myResult)
{
    this->myResult = myResult;
}

bool Beacon::getNeedCRL() const
{
    return this->NeedCRL;
}

void Beacon::setNeedCRL(bool NeedCRL)
{
    this->NeedCRL = NeedCRL;
}

int Beacon::getCRL_Piece_No() const
{
    return this->CRL_Piece_No;
}

void Beacon::setCRL_Piece_No(int CRL_Piece_No)
{
    this->CRL_Piece_No = CRL_Piece_No;
}

int Beacon::getRangeS() const
{
    return this->rangeS;
}

void Beacon::setRangeS(int rangeS)
{
    this->rangeS = rangeS;
}

int Beacon::getRangeE() const
{
    return this->rangeE;
}

void Beacon::setRangeE(int rangeE)
{
    this->rangeE = rangeE;
}

int Beacon::getCRLchannel() const
{
    return this->CRLchannel;
}

void Beacon::setCRLchannel(int CRLchannel)
{
    this->CRLchannel = CRLchannel;
}

class BeaconDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    BeaconDescriptor();
    virtual ~BeaconDescriptor();

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

Register_ClassDescriptor(BeaconDescriptor)

BeaconDescriptor::BeaconDescriptor() : omnetpp::cClassDescriptor("veins::Beacon", "veins::BaseFrame1609_4")
{
    propertynames = nullptr;
}

BeaconDescriptor::~BeaconDescriptor()
{
    delete[] propertynames;
}

bool BeaconDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Beacon *>(obj)!=nullptr;
}

const char **BeaconDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BeaconDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BeaconDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount() : 12;
}

unsigned int BeaconDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *BeaconDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "destAddr",
        "srcAddr",
        "NodeName",
        "positionX",
        "positionY",
        "speed",
        "myResult",
        "NeedCRL",
        "CRL_Piece_No",
        "rangeS",
        "rangeE",
        "CRLchannel",
    };
    return (field>=0 && field<12) ? fieldNames[field] : nullptr;
}

int BeaconDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddr")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+1;
    if (fieldName[0]=='N' && strcmp(fieldName, "NodeName")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionX")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "positionY")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+5;
    if (fieldName[0]=='m' && strcmp(fieldName, "myResult")==0) return base+6;
    if (fieldName[0]=='N' && strcmp(fieldName, "NeedCRL")==0) return base+7;
    if (fieldName[0]=='C' && strcmp(fieldName, "CRL_Piece_No")==0) return base+8;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangeS")==0) return base+9;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangeE")==0) return base+10;
    if (fieldName[0]=='C' && strcmp(fieldName, "CRLchannel")==0) return base+11;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BeaconDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "LAddress::L2Type",
        "LAddress::L2Type",
        "string",
        "double",
        "double",
        "double",
        "int",
        "bool",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : nullptr;
}

const char **BeaconDescriptor::getFieldPropertyNames(int field) const
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

const char *BeaconDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int BeaconDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *BeaconDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BeaconDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getDestAddr(); return out.str();}
        case 1: {std::stringstream out; out << pp->getSrcAddr(); return out.str();}
        case 2: return oppstring2string(pp->getNodeName());
        case 3: return double2string(pp->getPositionX());
        case 4: return double2string(pp->getPositionY());
        case 5: return double2string(pp->getSpeed());
        case 6: return long2string(pp->getMyResult());
        case 7: return bool2string(pp->getNeedCRL());
        case 8: return long2string(pp->getCRL_Piece_No());
        case 9: return long2string(pp->getRangeS());
        case 10: return long2string(pp->getRangeE());
        case 11: return long2string(pp->getCRLchannel());
        default: return "";
    }
}

bool BeaconDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        case 2: pp->setNodeName((value)); return true;
        case 3: pp->setPositionX(string2double(value)); return true;
        case 4: pp->setPositionY(string2double(value)); return true;
        case 5: pp->setSpeed(string2double(value)); return true;
        case 6: pp->setMyResult(string2long(value)); return true;
        case 7: pp->setNeedCRL(string2bool(value)); return true;
        case 8: pp->setCRL_Piece_No(string2long(value)); return true;
        case 9: pp->setRangeS(string2long(value)); return true;
        case 10: pp->setRangeE(string2long(value)); return true;
        case 11: pp->setCRLchannel(string2long(value)); return true;
        default: return false;
    }
}

const char *BeaconDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(LAddress::L2Type));
        case 1: return omnetpp::opp_typename(typeid(LAddress::L2Type));
        default: return nullptr;
    };
}

void *BeaconDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Beacon *pp = (Beacon *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getDestAddr()); break;
        case 1: return (void *)(&pp->getSrcAddr()); break;
        default: return nullptr;
    }
}

} // namespace veins

