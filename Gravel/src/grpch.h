#pragma once

#include "Engine/Core/Log.h"
#include "Engine/Core/Core.h"

#include "Engine/Debug/Instrumentor.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>


// platform specific headers
#ifdef GR_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

