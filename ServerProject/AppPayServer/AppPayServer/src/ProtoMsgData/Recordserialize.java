// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: recordserialize.proto

package ProtoMsgData;

public final class Recordserialize {
  private Recordserialize() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface RecordSerializeOrBuilder
      extends com.google.protobuf.MessageOrBuilder {
  }
  public static final class RecordSerialize extends
      com.google.protobuf.GeneratedMessage
      implements RecordSerializeOrBuilder {
    // Use RecordSerialize.newBuilder() to construct.
    private RecordSerialize(Builder builder) {
      super(builder);
    }
    private RecordSerialize(boolean noInit) {}
    
    private static final RecordSerialize defaultInstance;
    public static RecordSerialize getDefaultInstance() {
      return defaultInstance;
    }
    
    public RecordSerialize getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return ProtoMsgData.Recordserialize.internal_static_ProtoMsgData_RecordSerialize_descriptor;
    }
    
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return ProtoMsgData.Recordserialize.internal_static_ProtoMsgData_RecordSerialize_fieldAccessorTable;
    }
    
    private void initFields() {
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      memoizedIsInitialized = 1;
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      getUnknownFields().writeTo(output);
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      size += getUnknownFields().getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }
    
    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }
    
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static ProtoMsgData.Recordserialize.RecordSerialize parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(ProtoMsgData.Recordserialize.RecordSerialize prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements ProtoMsgData.Recordserialize.RecordSerializeOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return ProtoMsgData.Recordserialize.internal_static_ProtoMsgData_RecordSerialize_descriptor;
      }
      
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return ProtoMsgData.Recordserialize.internal_static_ProtoMsgData_RecordSerialize_fieldAccessorTable;
      }
      
      // Construct using ProtoMsgData.Recordserialize.RecordSerialize.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }
      
      private Builder(BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        }
      }
      private static Builder create() {
        return new Builder();
      }
      
      public Builder clear() {
        super.clear();
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return ProtoMsgData.Recordserialize.RecordSerialize.getDescriptor();
      }
      
      public ProtoMsgData.Recordserialize.RecordSerialize getDefaultInstanceForType() {
        return ProtoMsgData.Recordserialize.RecordSerialize.getDefaultInstance();
      }
      
      public ProtoMsgData.Recordserialize.RecordSerialize build() {
        ProtoMsgData.Recordserialize.RecordSerialize result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private ProtoMsgData.Recordserialize.RecordSerialize buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        ProtoMsgData.Recordserialize.RecordSerialize result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public ProtoMsgData.Recordserialize.RecordSerialize buildPartial() {
        ProtoMsgData.Recordserialize.RecordSerialize result = new ProtoMsgData.Recordserialize.RecordSerialize(this);
        onBuilt();
        return result;
      }
      
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof ProtoMsgData.Recordserialize.RecordSerialize) {
          return mergeFrom((ProtoMsgData.Recordserialize.RecordSerialize)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }
      
      public Builder mergeFrom(ProtoMsgData.Recordserialize.RecordSerialize other) {
        if (other == ProtoMsgData.Recordserialize.RecordSerialize.getDefaultInstance()) return this;
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }
      
      public final boolean isInitialized() {
        return true;
      }
      
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder(
            this.getUnknownFields());
        while (true) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              this.setUnknownFields(unknownFields.build());
              onChanged();
              return this;
            default: {
              if (!parseUnknownField(input, unknownFields,
                                     extensionRegistry, tag)) {
                this.setUnknownFields(unknownFields.build());
                onChanged();
                return this;
              }
              break;
            }
          }
        }
      }
      
      
      // @@protoc_insertion_point(builder_scope:ProtoMsgData.RecordSerialize)
    }
    
    static {
      defaultInstance = new RecordSerialize(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:ProtoMsgData.RecordSerialize)
  }
  
  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_ProtoMsgData_RecordSerialize_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_ProtoMsgData_RecordSerialize_fieldAccessorTable;
  
  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\025recordserialize.proto\022\014ProtoMsgData\"\021\n" +
      "\017RecordSerialize"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_ProtoMsgData_RecordSerialize_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_ProtoMsgData_RecordSerialize_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_ProtoMsgData_RecordSerialize_descriptor,
              new java.lang.String[] { },
              ProtoMsgData.Recordserialize.RecordSerialize.class,
              ProtoMsgData.Recordserialize.RecordSerialize.Builder.class);
          return null;
        }
      };
    com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        }, assigner);
  }
  
  // @@protoc_insertion_point(outer_class_scope)
}
