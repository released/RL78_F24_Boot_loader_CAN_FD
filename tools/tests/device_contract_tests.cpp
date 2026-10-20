/* Execute production CRC/metadata/block helpers on Windows (32-bit long).
 * This is a RAM/protocol model, not CAN/RFD/power-cut hardware verification. */
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#define __far
#define F13_CRC_HOST_TEST
#include "can_boot_transport.h"
#include "sw_crc32.c"
#include "can_boot_metadata.inc"
#include "can_boot_block.inc"
#ifdef F24_TEST_TOOL
#include "firmware_update_profile.h"
#endif

int main(int argc, char** argv) {
    static_assert(sizeof(unsigned long)==4, "RL78 words require 32-bit long");
    if (argc!=2) return 2;
    std::ifstream file(argv[1],std::ios::binary);
    const std::vector<unsigned char> image(std::istreambuf_iterator<char>(file),{});
    assert(image.size()>=516);
    std::array<unsigned long,64> words{};
    std::memcpy(words.data(),image.data()+256,256);
    assert(MetadataValid(words.data()));
    auto enabled=words; enabled[29]=1UL; assert(MetadataValid(enabled.data()));
    enabled[29]=2UL; assert(!MetadataValid(enabled.data()));
    for(unsigned policy:{0U,1U,3U}) for(unsigned long version:{0x10000UL,0x10001UL,0x10100UL}) {
        g_metadata_words[4]=version;
        assert(VersionAllowed(policy,0x10001UL,CAN_BOOT_CMD_START)==(policy!=3U || version>=0x10001UL));
        assert(VersionAllowed(policy,0x10001UL,CAN_BOOT_CMD_FORCE_START));
    }
    unsigned long stored=0;
    std::memcpy(&stored,image.data()+image.size()-4,4);
    assert(F13_Crc32Bytes(image.data(),static_cast<unsigned long>(image.size()-4))==stored);
    assert(F13_Crc32Bytes(reinterpret_cast<const unsigned char*>("123456789"),9)==0xCBF43926UL);
    for (unsigned n=0;n<64;++n) {
        auto invalid=words;
        if (n==4 || n==5 || n==6 || n==25 || n==27) continue;
        invalid[n]^=0x01000000;
        assert(!MetadataValid(invalid.data()));
    }
    for (auto value : {0x02012001UL,0x04012001UL,0x01022001UL}) {
        auto invalid=words; invalid[25]=value;
        assert(!MetadataValid(invalid.data()));
    }
    auto invalid=words; invalid[27]=IMAGE_BOOT_VERSION+1;
    assert(!MetadataValid(invalid.data()));
    for (auto size : {0UL,512UL,517UL,IMAGE_APPLICATION_CAPACITY+4}) {
        invalid=words; invalid[6]=size; assert(!MetadataValid(invalid.data()));
    }
    const auto* bytes=reinterpret_cast<const unsigned char*>(words.data());
    const auto crc=F13_Crc32Bytes(bytes,256);
    MetadataReset();
    assert(MetadataStage(4,bytes+4)==CAN_BOOT_STATUS_BAD_SEQUENCE);
    assert(MetadataSeal(crc)==CAN_BOOT_STATUS_BAD_IMAGE);
    for (unsigned offset=0;offset<256;offset+=4) {
        assert(MetadataStage(static_cast<unsigned short>(offset),bytes+offset)==0);
        assert(MetadataStage(static_cast<unsigned short>(offset),bytes+offset)==0);
    }
    assert(MetadataSeal(crc^1)==CAN_BOOT_STATUS_CRC_ERROR);
    assert(!g_metadata_sealed);
    assert(MetadataSeal(crc)==0 && g_metadata_sealed);
    unsigned char conflict[8]={0,0,0,0,0,0,0,0};
    assert(MetadataStage(0,conflict)==CAN_BOOT_STATUS_BAD_IMAGE);
    std::array<unsigned char,64> data{};
    for (unsigned i=0;i<data.size();++i) data[i]=static_cast<unsigned char>(i);
    for (unsigned tail=4;tail<=64;tail+=4) {
        const auto block_crc=F13_Crc32Bytes(data.data(),tail);
        BlockBegin(0,block_crc,0,tail);
        assert(BlockCheck(0,block_crc)!=0);
        auto padded=data;
        std::fill(padded.begin()+tail,padded.end(),static_cast<unsigned char>(0xFFU));
        BlockSegment(0,64,padded.data());
        BlockSegment(0,64,padded.data());
        assert(BlockCheck(0,block_crc)==0);
        assert(!std::memcmp(data.data(),g_block_words,tail));
        assert(BlockCheck(64,block_crc)!=0);
        BlockBegin(0,block_crc,0,tail);
        BlockSegment(0,64,padded.data());
        padded[0]^=1;
        BlockSegment(0,64,padded.data());
        assert(BlockCheck(0,block_crc)!=0);
        BlockBegin(0,block_crc,0,tail);
        BlockSegment(1,64,padded.data());
        assert(BlockCheck(0,block_crc)!=0);
        BlockBegin(0,block_crc,0,tail);
        BlockSegment(0,8,padded.data());
        assert(BlockCheck(0,block_crc)!=0);
        if(tail<64) {
            padded=data; padded[tail]=0;
            BlockBegin(0,block_crc,0,tail);
            BlockSegment(0,64,padded.data());
            assert(BlockCheck(0,block_crc)!=0);
        }
    }
    const auto block_crc=F13_Crc32Bytes(data.data(),64);
    BlockBegin(0,block_crc,64,128); assert(!g_block_bad);
    BlockBegin(0,block_crc,128,192); assert(g_block_bad);
    BlockBegin(64,block_crc,0,128); assert(g_block_bad);
    BlockBegin(4,block_crc,4,128); assert(g_block_bad);
    BlockBegin(0,block_crc^1,0,64);
    BlockSegment(0,64,data.data());
    assert(BlockCheck(0,block_crc^1)==CAN_BOOT_STATUS_CRC_ERROR);
#ifdef F24_TEST_TOOL
    using namespace firmware_upgrade;
    firmware_update::ImageInfo info; std::wstring error;
    assert(firmware_update::ValidateImage(firmware_update::Profile::CanUpgrade,image,&info,&error));
    assert(info.target_id==0xF24B && info.application_start==0x2000 && info.application_capacity==0x3DC00);
    assert(info.policy.irq_abi==IMAGE_IRQ_ABI && info.mcu_part==L"R7F124FPJ5xFB");
    assert(info.can_mode_mask==2 && info.nominal_bitrate_mask==4 && info.data_bitrate_mask==2);
    assert(image_policy::Allows(info.policy,IP_ROLE_DEVICE_APP,IP_CAN,&error));
    assert(!image_policy::Allows(info.policy,IP_ROLE_DEVICE_APP,IP_I2C,&error));
    assert(!image_policy::Allows(info.policy,IP_ROLE_BRIDGE_APP,IP_UART,&error));
    auto damaged=image; damaged.back()^=1;
    assert(!firmware_update::ValidateImage(firmware_update::Profile::CanUpgrade,damaged,&info,&error));
    std::cout << "Production Tool parser / F24 policy: PASS\n";
#endif
    std::cout << "Actual F24 BIN + production metadata/CRC/block RAM fault models: PASS\n";
}
