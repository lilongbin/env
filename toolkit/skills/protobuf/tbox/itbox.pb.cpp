// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: itbox.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "itbox.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace RadioInterfaceTBox {

namespace {

const ::google::protobuf::Descriptor* RadioTxCommunicationSyncNotify_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  RadioTxCommunicationSyncNotify_reflection_ = NULL;
const ::google::protobuf::Descriptor* RadioInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  RadioInfo_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* CommandID_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* MessageHeaderFlag_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_itbox_2eproto() {
  protobuf_AddDesc_itbox_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "itbox.proto");
  GOOGLE_CHECK(file != NULL);
  RadioTxCommunicationSyncNotify_descriptor_ = file->message_type(0);
  static const int RadioTxCommunicationSyncNotify_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioTxCommunicationSyncNotify, sync_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioTxCommunicationSyncNotify, radiobasicinfo_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioTxCommunicationSyncNotify, radioinformation_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioTxCommunicationSyncNotify, syncserialnumber_),
  };
  RadioTxCommunicationSyncNotify_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      RadioTxCommunicationSyncNotify_descriptor_,
      RadioTxCommunicationSyncNotify::default_instance_,
      RadioTxCommunicationSyncNotify_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioTxCommunicationSyncNotify, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioTxCommunicationSyncNotify, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(RadioTxCommunicationSyncNotify));
  RadioInfo_descriptor_ = file->message_type(1);
  static const int RadioInfo_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioInfo, radiohmiversion_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioInfo, radiofortboxserviceversion_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioInfo, communicationprotocolversion_),
  };
  RadioInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      RadioInfo_descriptor_,
      RadioInfo::default_instance_,
      RadioInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RadioInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(RadioInfo));
  CommandID_descriptor_ = file->enum_type(0);
  MessageHeaderFlag_descriptor_ = file->enum_type(1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_itbox_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    RadioTxCommunicationSyncNotify_descriptor_, &RadioTxCommunicationSyncNotify::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    RadioInfo_descriptor_, &RadioInfo::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_itbox_2eproto() {
  delete RadioTxCommunicationSyncNotify::default_instance_;
  delete RadioTxCommunicationSyncNotify_reflection_;
  delete RadioInfo::default_instance_;
  delete RadioInfo_reflection_;
}

void protobuf_AddDesc_itbox_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013itbox.proto\022\022RadioInterfaceTBox\"\231\001\n\036Ra"
    "dioTxCommunicationSyncNotify\022\014\n\004Sync\030\001 \002"
    "(\010\0225\n\016RadioBasicInfo\030\002 \002(\0132\035.RadioInterf"
    "aceTBox.RadioInfo\022\030\n\020RadioInformation\030\003 "
    "\002(\t\022\030\n\020SyncSerialNumber\030\004 \002(\r\"n\n\tRadioIn"
    "fo\022\027\n\017RadioHMIVersion\030\001 \002(\t\022\"\n\032RadioForT"
    "BoxServiceVersion\030\002 \002(\t\022$\n\034Communication"
    "ProtocolVersion\030\003 \002(\t*\245\002\n\tCommandID\022\'\n#e"
    "Cmd_RadioTxCommunicationSyncNotify\020\000\022+\n\'"
    "eCmd_TBoxTxCommunicationHandShakeNotify\020"
    "\001\022)\n%eCmd_RadioTxGetTBoxSupportFunctionR"
    "eq\020\002\022(\n$eCmd_TBoxTxGetTBoxSupportFunctio"
    "nAck\020\003\022\037\n\033eCmd_TBoxTxTBoxKeepAliveReq\020\004\022"
    " \n\034eCmd_RadioTxTBoxKeepAliveAck\020\005\022*\n&eCm"
    "d_RadioTxCommunicationGoodbyeNotify\020\006*^\n"
    "\021MessageHeaderFlag\022\026\n\022MHF_MsgTypeRequest"
    "\020\001\022\024\n\020MHF_MsgTypeReply\020\002\022\033\n\027MHF_MsgTypeN"
    "otification\020\004", 693);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "itbox.proto", &protobuf_RegisterTypes);
  RadioTxCommunicationSyncNotify::default_instance_ = new RadioTxCommunicationSyncNotify();
  RadioInfo::default_instance_ = new RadioInfo();
  RadioTxCommunicationSyncNotify::default_instance_->InitAsDefaultInstance();
  RadioInfo::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_itbox_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_itbox_2eproto {
  StaticDescriptorInitializer_itbox_2eproto() {
    protobuf_AddDesc_itbox_2eproto();
  }
} static_descriptor_initializer_itbox_2eproto_;
const ::google::protobuf::EnumDescriptor* CommandID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CommandID_descriptor_;
}
bool CommandID_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* MessageHeaderFlag_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MessageHeaderFlag_descriptor_;
}
bool MessageHeaderFlag_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
    case 4:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int RadioTxCommunicationSyncNotify::kSyncFieldNumber;
const int RadioTxCommunicationSyncNotify::kRadioBasicInfoFieldNumber;
const int RadioTxCommunicationSyncNotify::kRadioInformationFieldNumber;
const int RadioTxCommunicationSyncNotify::kSyncSerialNumberFieldNumber;
#endif  // !_MSC_VER

RadioTxCommunicationSyncNotify::RadioTxCommunicationSyncNotify()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
}

void RadioTxCommunicationSyncNotify::InitAsDefaultInstance() {
  radiobasicinfo_ = const_cast< ::RadioInterfaceTBox::RadioInfo*>(&::RadioInterfaceTBox::RadioInfo::default_instance());
}

RadioTxCommunicationSyncNotify::RadioTxCommunicationSyncNotify(const RadioTxCommunicationSyncNotify& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
}

void RadioTxCommunicationSyncNotify::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  sync_ = false;
  radiobasicinfo_ = NULL;
  radioinformation_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  syncserialnumber_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RadioTxCommunicationSyncNotify::~RadioTxCommunicationSyncNotify() {
  // @@protoc_insertion_point(destructor:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
  SharedDtor();
}

void RadioTxCommunicationSyncNotify::SharedDtor() {
  if (radioinformation_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete radioinformation_;
  }
  if (this != default_instance_) {
    delete radiobasicinfo_;
  }
}

void RadioTxCommunicationSyncNotify::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* RadioTxCommunicationSyncNotify::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RadioTxCommunicationSyncNotify_descriptor_;
}

const RadioTxCommunicationSyncNotify& RadioTxCommunicationSyncNotify::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_itbox_2eproto();
  return *default_instance_;
}

RadioTxCommunicationSyncNotify* RadioTxCommunicationSyncNotify::default_instance_ = NULL;

RadioTxCommunicationSyncNotify* RadioTxCommunicationSyncNotify::New() const {
  return new RadioTxCommunicationSyncNotify;
}

void RadioTxCommunicationSyncNotify::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<RadioTxCommunicationSyncNotify*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  if (_has_bits_[0 / 32] & 15) {
    ZR_(sync_, syncserialnumber_);
    if (has_radiobasicinfo()) {
      if (radiobasicinfo_ != NULL) radiobasicinfo_->::RadioInterfaceTBox::RadioInfo::Clear();
    }
    if (has_radioinformation()) {
      if (radioinformation_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        radioinformation_->clear();
      }
    }
  }

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool RadioTxCommunicationSyncNotify::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required bool Sync = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &sync_)));
          set_has_sync();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_RadioBasicInfo;
        break;
      }

      // required .RadioInterfaceTBox.RadioInfo RadioBasicInfo = 2;
      case 2: {
        if (tag == 18) {
         parse_RadioBasicInfo:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_radiobasicinfo()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_RadioInformation;
        break;
      }

      // required string RadioInformation = 3;
      case 3: {
        if (tag == 26) {
         parse_RadioInformation:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_radioinformation()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->radioinformation().data(), this->radioinformation().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "radioinformation");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_SyncSerialNumber;
        break;
      }

      // required uint32 SyncSerialNumber = 4;
      case 4: {
        if (tag == 32) {
         parse_SyncSerialNumber:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &syncserialnumber_)));
          set_has_syncserialnumber();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
  return false;
#undef DO_
}

void RadioTxCommunicationSyncNotify::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
  // required bool Sync = 1;
  if (has_sync()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->sync(), output);
  }

  // required .RadioInterfaceTBox.RadioInfo RadioBasicInfo = 2;
  if (has_radiobasicinfo()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->radiobasicinfo(), output);
  }

  // required string RadioInformation = 3;
  if (has_radioinformation()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->radioinformation().data(), this->radioinformation().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "radioinformation");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      3, this->radioinformation(), output);
  }

  // required uint32 SyncSerialNumber = 4;
  if (has_syncserialnumber()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->syncserialnumber(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
}

::google::protobuf::uint8* RadioTxCommunicationSyncNotify::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
  // required bool Sync = 1;
  if (has_sync()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(1, this->sync(), target);
  }

  // required .RadioInterfaceTBox.RadioInfo RadioBasicInfo = 2;
  if (has_radiobasicinfo()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->radiobasicinfo(), target);
  }

  // required string RadioInformation = 3;
  if (has_radioinformation()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->radioinformation().data(), this->radioinformation().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "radioinformation");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->radioinformation(), target);
  }

  // required uint32 SyncSerialNumber = 4;
  if (has_syncserialnumber()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->syncserialnumber(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:RadioInterfaceTBox.RadioTxCommunicationSyncNotify)
  return target;
}

int RadioTxCommunicationSyncNotify::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required bool Sync = 1;
    if (has_sync()) {
      total_size += 1 + 1;
    }

    // required .RadioInterfaceTBox.RadioInfo RadioBasicInfo = 2;
    if (has_radiobasicinfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->radiobasicinfo());
    }

    // required string RadioInformation = 3;
    if (has_radioinformation()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->radioinformation());
    }

    // required uint32 SyncSerialNumber = 4;
    if (has_syncserialnumber()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->syncserialnumber());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RadioTxCommunicationSyncNotify::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const RadioTxCommunicationSyncNotify* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const RadioTxCommunicationSyncNotify*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void RadioTxCommunicationSyncNotify::MergeFrom(const RadioTxCommunicationSyncNotify& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_sync()) {
      set_sync(from.sync());
    }
    if (from.has_radiobasicinfo()) {
      mutable_radiobasicinfo()->::RadioInterfaceTBox::RadioInfo::MergeFrom(from.radiobasicinfo());
    }
    if (from.has_radioinformation()) {
      set_radioinformation(from.radioinformation());
    }
    if (from.has_syncserialnumber()) {
      set_syncserialnumber(from.syncserialnumber());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void RadioTxCommunicationSyncNotify::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void RadioTxCommunicationSyncNotify::CopyFrom(const RadioTxCommunicationSyncNotify& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RadioTxCommunicationSyncNotify::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;

  if (has_radiobasicinfo()) {
    if (!this->radiobasicinfo().IsInitialized()) return false;
  }
  return true;
}

void RadioTxCommunicationSyncNotify::Swap(RadioTxCommunicationSyncNotify* other) {
  if (other != this) {
    std::swap(sync_, other->sync_);
    std::swap(radiobasicinfo_, other->radiobasicinfo_);
    std::swap(radioinformation_, other->radioinformation_);
    std::swap(syncserialnumber_, other->syncserialnumber_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata RadioTxCommunicationSyncNotify::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = RadioTxCommunicationSyncNotify_descriptor_;
  metadata.reflection = RadioTxCommunicationSyncNotify_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int RadioInfo::kRadioHMIVersionFieldNumber;
const int RadioInfo::kRadioForTBoxServiceVersionFieldNumber;
const int RadioInfo::kCommunicationProtocolVersionFieldNumber;
#endif  // !_MSC_VER

RadioInfo::RadioInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:RadioInterfaceTBox.RadioInfo)
}

void RadioInfo::InitAsDefaultInstance() {
}

RadioInfo::RadioInfo(const RadioInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:RadioInterfaceTBox.RadioInfo)
}

void RadioInfo::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  radiohmiversion_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  radiofortboxserviceversion_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  communicationprotocolversion_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RadioInfo::~RadioInfo() {
  // @@protoc_insertion_point(destructor:RadioInterfaceTBox.RadioInfo)
  SharedDtor();
}

void RadioInfo::SharedDtor() {
  if (radiohmiversion_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete radiohmiversion_;
  }
  if (radiofortboxserviceversion_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete radiofortboxserviceversion_;
  }
  if (communicationprotocolversion_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete communicationprotocolversion_;
  }
  if (this != default_instance_) {
  }
}

void RadioInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* RadioInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RadioInfo_descriptor_;
}

const RadioInfo& RadioInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_itbox_2eproto();
  return *default_instance_;
}

RadioInfo* RadioInfo::default_instance_ = NULL;

RadioInfo* RadioInfo::New() const {
  return new RadioInfo;
}

void RadioInfo::Clear() {
  if (_has_bits_[0 / 32] & 7) {
    if (has_radiohmiversion()) {
      if (radiohmiversion_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        radiohmiversion_->clear();
      }
    }
    if (has_radiofortboxserviceversion()) {
      if (radiofortboxserviceversion_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        radiofortboxserviceversion_->clear();
      }
    }
    if (has_communicationprotocolversion()) {
      if (communicationprotocolversion_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        communicationprotocolversion_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool RadioInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:RadioInterfaceTBox.RadioInfo)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string RadioHMIVersion = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_radiohmiversion()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->radiohmiversion().data(), this->radiohmiversion().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "radiohmiversion");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_RadioForTBoxServiceVersion;
        break;
      }

      // required string RadioForTBoxServiceVersion = 2;
      case 2: {
        if (tag == 18) {
         parse_RadioForTBoxServiceVersion:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_radiofortboxserviceversion()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->radiofortboxserviceversion().data(), this->radiofortboxserviceversion().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "radiofortboxserviceversion");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_CommunicationProtocolVersion;
        break;
      }

      // required string CommunicationProtocolVersion = 3;
      case 3: {
        if (tag == 26) {
         parse_CommunicationProtocolVersion:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_communicationprotocolversion()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->communicationprotocolversion().data(), this->communicationprotocolversion().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "communicationprotocolversion");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:RadioInterfaceTBox.RadioInfo)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:RadioInterfaceTBox.RadioInfo)
  return false;
#undef DO_
}

void RadioInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:RadioInterfaceTBox.RadioInfo)
  // required string RadioHMIVersion = 1;
  if (has_radiohmiversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->radiohmiversion().data(), this->radiohmiversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "radiohmiversion");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->radiohmiversion(), output);
  }

  // required string RadioForTBoxServiceVersion = 2;
  if (has_radiofortboxserviceversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->radiofortboxserviceversion().data(), this->radiofortboxserviceversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "radiofortboxserviceversion");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->radiofortboxserviceversion(), output);
  }

  // required string CommunicationProtocolVersion = 3;
  if (has_communicationprotocolversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->communicationprotocolversion().data(), this->communicationprotocolversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "communicationprotocolversion");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      3, this->communicationprotocolversion(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:RadioInterfaceTBox.RadioInfo)
}

::google::protobuf::uint8* RadioInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:RadioInterfaceTBox.RadioInfo)
  // required string RadioHMIVersion = 1;
  if (has_radiohmiversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->radiohmiversion().data(), this->radiohmiversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "radiohmiversion");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->radiohmiversion(), target);
  }

  // required string RadioForTBoxServiceVersion = 2;
  if (has_radiofortboxserviceversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->radiofortboxserviceversion().data(), this->radiofortboxserviceversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "radiofortboxserviceversion");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->radiofortboxserviceversion(), target);
  }

  // required string CommunicationProtocolVersion = 3;
  if (has_communicationprotocolversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->communicationprotocolversion().data(), this->communicationprotocolversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "communicationprotocolversion");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->communicationprotocolversion(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:RadioInterfaceTBox.RadioInfo)
  return target;
}

int RadioInfo::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string RadioHMIVersion = 1;
    if (has_radiohmiversion()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->radiohmiversion());
    }

    // required string RadioForTBoxServiceVersion = 2;
    if (has_radiofortboxserviceversion()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->radiofortboxserviceversion());
    }

    // required string CommunicationProtocolVersion = 3;
    if (has_communicationprotocolversion()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->communicationprotocolversion());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RadioInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const RadioInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const RadioInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void RadioInfo::MergeFrom(const RadioInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_radiohmiversion()) {
      set_radiohmiversion(from.radiohmiversion());
    }
    if (from.has_radiofortboxserviceversion()) {
      set_radiofortboxserviceversion(from.radiofortboxserviceversion());
    }
    if (from.has_communicationprotocolversion()) {
      set_communicationprotocolversion(from.communicationprotocolversion());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void RadioInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void RadioInfo::CopyFrom(const RadioInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RadioInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void RadioInfo::Swap(RadioInfo* other) {
  if (other != this) {
    std::swap(radiohmiversion_, other->radiohmiversion_);
    std::swap(radiofortboxserviceversion_, other->radiofortboxserviceversion_);
    std::swap(communicationprotocolversion_, other->communicationprotocolversion_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata RadioInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = RadioInfo_descriptor_;
  metadata.reflection = RadioInfo_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace RadioInterfaceTBox

// @@protoc_insertion_point(global_scope)
