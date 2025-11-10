// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _INPUT_LISTENER_H
#define _INPUT_LISTENER_H

namespace pe {
    namespace intern {
        class InputListener {
        public:
            friend class InputEventDistributor;
        protected:
            bool _isKeyListener = false;
            bool _isMouseListener = false;
            bool _isGamepadListener = false;

            virtual void listenToAll() {
                _isKeyListener = true;
                _isMouseListener = true;
                _isGamepadListener = true;
            };
        };
    }
}

#endif