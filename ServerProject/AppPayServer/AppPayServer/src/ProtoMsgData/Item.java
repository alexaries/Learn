// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: item.proto

package ProtoMsgData;

public final class Item {
  private Item() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface ItemInfoOrBuilder
      extends com.google.protobuf.MessageOrBuilder {
    
    // required uint32 id = 1;
    boolean hasId();
    int getId();
    
    // required uint32 val = 2;
    boolean hasVal();
    int getVal();
  }
  public static final class ItemInfo extends
      com.google.protobuf.GeneratedMessage
      implements ItemInfoOrBuilder {
    // Use ItemInfo.newBuilder() to construct.
    private ItemInfo(Builder builder) {
      super(builder);
    }
    private ItemInfo(boolean noInit) {}
    
    private static final ItemInfo defaultInstance;
    public static ItemInfo getDefaultInstance() {
      return defaultInstance;
    }
    
    public ItemInfo getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return ProtoMsgData.Item.internal_static_ProtoMsgData_ItemInfo_descriptor;
    }
    
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return ProtoMsgData.Item.internal_static_ProtoMsgData_ItemInfo_fieldAccessorTable;
    }
    
    private int bitField0_;
    // required uint32 id = 1;
    public static final int ID_FIELD_NUMBER = 1;
    private int id_;
    public boolean hasId() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    public int getId() {
      return id_;
    }
    
    // required uint32 val = 2;
    public static final int VAL_FIELD_NUMBER = 2;
    private int val_;
    public boolean hasVal() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    public int getVal() {
      return val_;
    }
    
    private void initFields() {
      id_ = 0;
      val_ = 0;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      if (!hasId()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!hasVal()) {
        memoizedIsInitialized = 0;
        return false;
      }
      memoizedIsInitialized = 1;
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeUInt32(1, id_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeUInt32(2, val_);
      }
      getUnknownFields().writeTo(output);
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeUInt32Size(1, id_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeUInt32Size(2, val_);
      }
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
    
    public static ProtoMsgData.Item.ItemInfo parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static ProtoMsgData.Item.ItemInfo parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static ProtoMsgData.Item.ItemInfo parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static ProtoMsgData.Item.ItemInfo parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static ProtoMsgData.Item.ItemInfo parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static ProtoMsgData.Item.ItemInfo parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static ProtoMsgData.Item.ItemInfo parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static ProtoMsgData.Item.ItemInfo parseDelimitedFrom(
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
    public static ProtoMsgData.Item.ItemInfo parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static ProtoMsgData.Item.ItemInfo parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(ProtoMsgData.Item.ItemInfo prototype) {
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
       implements ProtoMsgData.Item.ItemInfoOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return ProtoMsgData.Item.internal_static_ProtoMsgData_ItemInfo_descriptor;
      }
      
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return ProtoMsgData.Item.internal_static_ProtoMsgData_ItemInfo_fieldAccessorTable;
      }
      
      // Construct using ProtoMsgData.Item.ItemInfo.newBuilder()
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
        id_ = 0;
        bitField0_ = (bitField0_ & ~0x00000001);
        val_ = 0;
        bitField0_ = (bitField0_ & ~0x00000002);
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return ProtoMsgData.Item.ItemInfo.getDescriptor();
      }
      
      public ProtoMsgData.Item.ItemInfo getDefaultInstanceForType() {
        return ProtoMsgData.Item.ItemInfo.getDefaultInstance();
      }
      
      public ProtoMsgData.Item.ItemInfo build() {
        ProtoMsgData.Item.ItemInfo result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private ProtoMsgData.Item.ItemInfo buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        ProtoMsgData.Item.ItemInfo result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public ProtoMsgData.Item.ItemInfo buildPartial() {
        ProtoMsgData.Item.ItemInfo result = new ProtoMsgData.Item.ItemInfo(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.id_ = id_;
        if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
          to_bitField0_ |= 0x00000002;
        }
        result.val_ = val_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }
      
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof ProtoMsgData.Item.ItemInfo) {
          return mergeFrom((ProtoMsgData.Item.ItemInfo)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }
      
      public Builder mergeFrom(ProtoMsgData.Item.ItemInfo other) {
        if (other == ProtoMsgData.Item.ItemInfo.getDefaultInstance()) return this;
        if (other.hasId()) {
          setId(other.getId());
        }
        if (other.hasVal()) {
          setVal(other.getVal());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }
      
      public final boolean isInitialized() {
        if (!hasId()) {
          
          return false;
        }
        if (!hasVal()) {
          
          return false;
        }
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
            case 8: {
              bitField0_ |= 0x00000001;
              id_ = input.readUInt32();
              break;
            }
            case 16: {
              bitField0_ |= 0x00000002;
              val_ = input.readUInt32();
              break;
            }
          }
        }
      }
      
      private int bitField0_;
      
      // required uint32 id = 1;
      private int id_ ;
      public boolean hasId() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      public int getId() {
        return id_;
      }
      public Builder setId(int value) {
        bitField0_ |= 0x00000001;
        id_ = value;
        onChanged();
        return this;
      }
      public Builder clearId() {
        bitField0_ = (bitField0_ & ~0x00000001);
        id_ = 0;
        onChanged();
        return this;
      }
      
      // required uint32 val = 2;
      private int val_ ;
      public boolean hasVal() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      public int getVal() {
        return val_;
      }
      public Builder setVal(int value) {
        bitField0_ |= 0x00000002;
        val_ = value;
        onChanged();
        return this;
      }
      public Builder clearVal() {
        bitField0_ = (bitField0_ & ~0x00000002);
        val_ = 0;
        onChanged();
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:ProtoMsgData.ItemInfo)
    }
    
    static {
      defaultInstance = new ItemInfo(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:ProtoMsgData.ItemInfo)
  }
  
  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_ProtoMsgData_ItemInfo_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_ProtoMsgData_ItemInfo_fieldAccessorTable;
  
  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\nitem.proto\022\014ProtoMsgData\"#\n\010ItemInfo\022\n" +
      "\n\002id\030\001 \002(\r\022\013\n\003val\030\002 \002(\r"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_ProtoMsgData_ItemInfo_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_ProtoMsgData_ItemInfo_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_ProtoMsgData_ItemInfo_descriptor,
              new java.lang.String[] { "Id", "Val", },
              ProtoMsgData.Item.ItemInfo.class,
              ProtoMsgData.Item.ItemInfo.Builder.class);
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