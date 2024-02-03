#include <fmt/format.h>

#include <vector>
#include <variant>
#include <optional>

template <typename ...Ts> struct Overload : public Ts... { using Ts::operator()...; };

enum class PACKET_TYPE : char {};

template <typename T>
class Endian
{
public:
    T get() const { return std::byteswap(value); }
private:
    T value;
};

#pragma pack(push, 1)
struct BaseStruct
{
    Endian<float> g;
};
#pragma pack(pop)

struct type1 
{
    static constexpr PACKET_TYPE CATAGORY{};
    static constexpr bool IS_TRACK = true;
    static constexpr int64_t PACKET_ID = 0b100;
    static constexpr bool VAR_LENGTH = true;
    float b;
};

struct type2 
{
    static constexpr PACKET_TYPE CATAGORY{};
    static constexpr bool IS_TRACK = true;
    static constexpr int64_t PACKET_ID = 0b100;
    static constexpr bool VAR_LENGTH = true;
};

class Packet1
{
public:
    std::optional<double> f1;


private:
    Packet1(std::vector<std::variant<type1, type2>> vec) 
    {
            for(const auto& v : vec)
                std::visit(Overload{
                    [&](type1 data){ f1 = data.b; },
                    [&](auto data){}
                }, v);
    };
    std::vector<std::variant<type1, type2>> marshall() const;
    friend class Factory;
};

template <typename ...Messages>
std::vector<std::variant<Messages...>> parse()
{
    char* data;
    struct Header
    {
        PACKET_TYPE cat{};
        int64_t packetID = 0; 
        bool isTrack;
    } head;

    for(int index = 0;;)
    {
        head.packetID <<= 8;
        head.packetID |= data[index];
    }

    char* item;
    std::vector<std::variant<Messages...>> ret;
    // Handle header
   while(head.packetID != 0)
   { 
        ([&]()
        {
            if(Messages::CATAGORY == head.cat && Messages::IS_TRACK == head.isTrack && Messages::PACKET_ID & head.packetID)
            {
                Messages* msg = reinterpret_cast<Messages*>(item);
                if constexpr (Messages::VAR_LENGTH)
                {
                    // Parse variable length
                }
                ret.emplace_back(*msg);
                head.packetID &= ~Messages::PACKET_ID;
                // advance item
                return true;
            }
            return false;
        }() || ...);
    }

    return ret;
}

int main()
{
    auto vec = parse<type1, type2>();

    for(const auto& v : vec)
        std::visit(Overload{
            [](type1 data){ fmt::println("type1"); },
            [](type2 data){ fmt::println("type2"); },
            [](auto def){ fmt::println("Idk type"); }
        }, v);
}
