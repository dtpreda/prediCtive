//
// Created by dtpreda on 02/08/22.
//

#include <memory>

#include "utils.h"

std::shared_ptr<Terminal> END_OF_INPUT = std::make_shared<Terminal>("EOF", "$");