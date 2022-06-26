#include <Gravel.h>
#include <Engine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Gravel {

	class Garden : public Application
	{
	public:
		Garden()
			: Application("GRAVEL::GARDEN")
		{
			AddLayer(new EditorLayer());
		}

		~Garden()
		{

		}
	};

	Application* CreateApplication()
	{
		return new Garden();
	}

}