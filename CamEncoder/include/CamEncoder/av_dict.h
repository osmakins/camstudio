/**
 * Copyright(C) 2018  Steven Hoving
 *
 * This program is free software : you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.If not, see < https://www.gnu.org/licenses/>.
 */

#pragma once

#include "av_ffmpeg.h"
#include <string_view>
#include <cassert>

// this is a wrapper around AVDictionary. The interface is somewhat modeled after std::map.
class av_dict
{
public:
    class av_mapped_type
    {
    public:
        av_mapped_type(AVDictionary **dict, const std::string_view &key) noexcept
            : dict_(dict)
            , key_(key)
        {
        }

        void operator=(const std::string_view &value)
        {
             av_dict_set(dict_, key_.data(), value.data(), 0);
        }

    private:
        AVDictionary **dict_;
        const std::string_view &key_;
    };

    ~av_dict()
    {
        av_dict_free(&dict_);
    }

    int size() const noexcept
    {
        return av_dict_count(dict_);
    }

    // \todo make this a bit mode std::map style...
    AVDictionaryEntry *at(const std::string_view &key, const AVDictionaryEntry *prev = nullptr, int flags = 0)
    {
        return av_dict_get(dict_, key.data(), prev, flags);
    }

    av_mapped_type operator[](const std::string_view &key) noexcept
    {
        return av_mapped_type(&dict_, key);
    }

    operator AVDictionary **() noexcept
    {
        assert(dict_ != nullptr);
        return &dict_;
    }

private:
    AVDictionary *dict_{nullptr};
};