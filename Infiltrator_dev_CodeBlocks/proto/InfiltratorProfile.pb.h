// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: InfiltratorProfile.proto

#ifndef PROTOBUF_InfiltratorProfile_2eproto__INCLUDED
#define PROTOBUF_InfiltratorProfile_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace Infiltrator {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_InfiltratorProfile_2eproto();
void protobuf_AssignDesc_InfiltratorProfile_2eproto();
void protobuf_ShutdownFile_InfiltratorProfile_2eproto();

class Profile;

// ===================================================================

class Profile : public ::google::protobuf::Message {
 public:
  Profile();
  virtual ~Profile();
  
  Profile(const Profile& from);
  
  inline Profile& operator=(const Profile& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Profile& default_instance();
  
  void Swap(Profile* other);
  
  // implements Message ----------------------------------------------
  
  Profile* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Profile& from);
  void MergeFrom(const Profile& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 allGold = 2;
  inline bool has_allgold() const;
  inline void clear_allgold();
  static const int kAllGoldFieldNumber = 2;
  inline ::google::protobuf::uint32 allgold() const;
  inline void set_allgold(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:Infiltrator.Profile)
 private:
  inline void set_has_allgold();
  inline void clear_has_allgold();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 allgold_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_InfiltratorProfile_2eproto();
  friend void protobuf_AssignDesc_InfiltratorProfile_2eproto();
  friend void protobuf_ShutdownFile_InfiltratorProfile_2eproto();
  
  void InitAsDefaultInstance();
  static Profile* default_instance_;
};
// ===================================================================


// ===================================================================

// Profile

// required uint32 allGold = 2;
inline bool Profile::has_allgold() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Profile::set_has_allgold() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Profile::clear_has_allgold() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Profile::clear_allgold() {
  allgold_ = 0u;
  clear_has_allgold();
}
inline ::google::protobuf::uint32 Profile::allgold() const {
  return allgold_;
}
inline void Profile::set_allgold(::google::protobuf::uint32 value) {
  set_has_allgold();
  allgold_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Infiltrator

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_InfiltratorProfile_2eproto__INCLUDED
