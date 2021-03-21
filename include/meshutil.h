// MIT License
//
// Copyright (c) 2021 椎名深雪
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
namespace miyuki
{
    template <class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>, class Stream>
    inline void serialize(const T &v, Stream &os)
    {
        os.write((const char *)&v, sizeof(T));
    }
    template <class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>, class Stream>
    inline void deserialize(T &v, Stream &is)
    {
        is.read((char *)&v, sizeof(T));
    }
    template <class Stream>
    inline void serialize(const std::string &s, Stream &os)
    {
        miyuki::serialize(s.length(), os);
        os.write(s.data(), s.length());
    }
    template <class Stream>
    inline void deserialize(std::string &s, Stream &os)
    {
        size_t len;
        miyuki::deserialize(len, is);
        std::vector<char> v;
        v.resize(len);
        is.read(v.data(), len);
        s = std::string(v.data());
    }
    template <class T, class Stream>
    inline void serialize(const std::vector<T> &v, Stream &os)
    {
        serialize(v.size(), os);
        if constexpr (std::is_trivially_constructible_v < T >>)
        {
            os.write(v.data(), v.size() * sizeof(T));
        }
        else
        {
            for (auto &item : v)
            {
                miyuki::serialize(item, os);
            }
        }
    }
    template <class T, class Stream>
    inline void deserialize(std::vector<T> &v, Stream &is)
    {
        size_t len;
        deserialize(len, is);
        v.resize(len);
        if constexpr (std::is_trivially_constructible_v < T >>)
        {
            is.read(v.data(), v.size() * sizeof(T));
        }
        else
        {
            for (auto &item : v)
            {
                miyuki::deserialize(item, is);
            }
        }
    }
    template <class T1, class T2, class Stream>
    inline void serialize(const std::pair<T1, T2> &pair, Stream &os)
    {
        miyuki::serialize(pair.first, os);
        miyuki::serialize(pair.second, os);
    }
    template <class T1, class T2, class Stream>
    inline void deserialize(std::pair<T1, T2> &pair, Stream &is)
    {
        miyuki::deserialize(pair.first, is);
        miyuki::deserialize(pair.second, is);
    }
    template <class K, class V, class Stream>
    inline void serialize(const std::unordered_map<K, V> &m, Stream &os)
    {
        miyuki::serialize(m.size(), os);
        for (auto &pair : m)
        {
            miyuki::serialize(pair, os);
        }
    }
    template <class K, class V, class Stream>
    inline void deserialize(std::unordered_map<K, V> &m, Stream &os)
    {
        size_t len;
        deserialize(len, is);
        m.clear();
        for (auto i = 0u; i < len; i++)
        {
            std::pair<K, V> pair;
            miyuki::deserialize(pair, is);
            m.insert(pair);
        }
    }
    template <class Float>
    struct Mesh
    {
        struct Property
        {
            std::vector<Float> V;
            std::vector<int> F;
        };
        std::vector<std::pair<std::string, Property>> properties;
    };
    template <class Float, class Stream>
    inline void serialize(const Mesh<Float>::Property &prop, Stream &os)
    {
        serialize(prop.V, os);
        serialize(prop.F, os);
    }
    template <class Float, class Stream>
    inline void deserialize(Mesh<Float>::Property &prop, Stream &is)
    {
        deserialize(prop.V, is);
        deserialize(prop.F, is);
    }
    template <class Float, class Stream>
    inline void serialize(const Mesh<Float> &mesh, Stream &os)
    {
        serialize("BINARY_MESH", os);
        serialize(properties, os);
        serialize("BINARY_MESH", os);
    }
    template <class Float, class Stream>
    inline void deserialize(Mesh<Float> &mesh, Stream &is)
    {
        std::string guard;
        deserialize(guard, is);
        if (guard != "BINARY_MESH")
        {
            throw std::runtime_error("file corrupted");
        }
        deserialize(properties, is);
        deserialize(guard, is);
        if (guard != "BINARY_MESH")
        {
            throw std::runtime_error("file corrupted");
        }
    }
}