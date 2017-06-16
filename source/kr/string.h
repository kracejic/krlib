#include <algorithm>
#include <string>
#include <vector>

namespace kr
{

namespace priv
{
    constexpr static const char* delims = " \t\r\n";

    struct itostr_helper
    {
        static unsigned out[10000];

        itostr_helper()
        {
            for (int i = 0; i < 10000; i++)
            {
                unsigned v = i;
                char* o = (char*)(out + i);
                o[3] = v % 10 + '0';
                o[2] = (v % 100) / 10 + '0';
                o[1] = (v % 1000) / 100 + '0';
                o[0] = (v % 10000) / 1000;
                if (o[0])
                    o[0] |= 0x30;
                else if (o[1] != '0')
                    o[0] |= 0x20;
                else if (o[2] != '0')
                    o[0] |= 0x10;
                else
                    o[0] |= 0x00;
            }
        }
    };
    unsigned itostr_helper::out[10000];

    itostr_helper hlp_init;
} /* private */

template <typename T>
std::string itostr(T o)
{
    typedef priv::itostr_helper hlp;

    unsigned blocks[3], *b = blocks + 2;
    blocks[0] = o < 0 ? ~o + 1 : o;
    blocks[2] = blocks[0] % 10000;
    blocks[0] /= 10000;
    blocks[2] = hlp::out[blocks[2]];

    if (blocks[0])
    {
        blocks[1] = blocks[0] % 10000;
        blocks[0] /= 10000;
        blocks[1] = hlp::out[blocks[1]];
        blocks[2] |= 0x30303030;
        b--;
    }

    if (blocks[0])
    {
        blocks[0] = hlp::out[blocks[0] % 10000];
        blocks[1] |= 0x30303030;
        b--;
    }

    char* f = ((char*)b);
    f += 3 - (*f >> 4);

    char* str = (char*)blocks;
    if (o < 0)
        *--f = '-';
    return std::string(f, (str + 12) - f);
}

std::string human_readable(int64_t size)
{
    if (size < 1024L * 10L)
        return itostr(size);
    if (size < 1024L * 1024L * 10L)
        return itostr(size / 1024L) + "k";
    if (size < 1024L * 1024L * 1024L * 10L)
        return itostr(size / 1024L / 1024L) + "M";
    if (size < 1024L * 1024L * 1024L * 1024L * 10L)
        return itostr(size / 1024L / 1024L / 1024L) + "G";
    return itostr(size / 1024L / 1024L / 1024L / 1024L) + "T";
}

/**
 * @return true if text starts with substring
 */
bool starts_with(const std::string& text, const std::string& substring)
{
    if (substring.size() > text.size())
        return false;
    return std::equal(substring.begin(), substring.end(), text.begin());
}

/**
 * @return true if text ends with substring
 */
bool ends_with(const std::string& text, const std::string& substring)
{
    if (substring.size() > text.size())
        return false;
    return std::equal(substring.rbegin(), substring.rend(), text.rbegin());
}


/**
 * modify string by triming whitespace
 */
void trim(std::string& text, bool left = true, bool right = true)
{
    if (right)
        text.erase(text.find_last_not_of(priv::delims) + 1);
    if (left)
        text.erase(0, text.find_first_not_of(priv::delims));
}

/**
 * @return trimmed string
 */
std::string trimmed(std::string text, bool left = true, bool right = true)
{
    if (right)
        text.erase(text.find_last_not_of(priv::delims) + 1);
    if (left)
        text.erase(0, text.find_first_not_of(priv::delims));
    return text;
}

/**
 * Splits the string.
 * @param text input string
 * @param delimiters delimiters for split
 * @param maxSplits max number of splits (first split does not count), max
 *                  number of results is maxSplits+1
 * @return vector of strings
 */
std::vector<std::string> split(const std::string& text,
    const std::string& delimiters = "\t\n ", int maxSplits = 0)
{
    std::vector<std::string> result;
    result.reserve(maxSplits ? maxSplits + 1 : 10); // quess
    int numSplits = 0;
    size_t start = 0, pos;

    do
    {
        pos = text.find_first_of(delimiters, start);
        if (pos == start)
        {
            start = pos + 1;
            numSplits--; // first split does not count
        }
        else if (pos == std::string::npos ||
                 (maxSplits && numSplits == maxSplits))
        {
            // Copy the rest of the string
            result.emplace_back(text.substr(start));
            return result;
        }
        else
        {
            // Copy up to delimiter
            result.emplace_back(text.substr(start, pos - start));
            start = pos + 1;
        }
        // parse up to next real data
        start = text.find_first_not_of(delimiters, start);
        ++numSplits;
    } while (pos != std::string::npos);

    return result;
}

void transformToLowerCase(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), tolower);
}

void transformToUpperCase(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), toupper);
}

std::string toLowerCase(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), tolower);
    return text;
}

std::string toUpperCase(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), toupper);
    return text;
}
} /* kr */
