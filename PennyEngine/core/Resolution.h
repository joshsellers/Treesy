// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _RESOLUTION_H
#define _RESOLUTION_H

namespace pe {
    struct Resolution {
        Resolution(const int width = 0, const int height = 0) : width(width), height(height) {}

        int width;
        int height;

        bool operator==(Resolution other) const {
            return this->width == other.width && this->height == other.height;
        }

        bool operator!=(Resolution other) const {
            return this->width != other.width || this->height != other.height;
        }
    };
}

#endif