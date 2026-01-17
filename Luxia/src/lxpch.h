#pragma once

// Standard
#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <stdio.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

// Luxia API
#include "Luxia/Core/Log.h"
#include "Luxia/Tools/Constants.h"

// Math
#include "glm/common.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>



#ifdef LX_PLATFORM_WINDOWS
	#include <Windows.h>
#endif