# Luxia

## What is it?
### Background
Luxia is the successor to my "StinkerBug" Game engine. While it was an amazing project I noticed many flaws. Firstly it wasn't properly formatted. Everything, Engine, Editor, etc was one solution. And that solution was in one folder. I also didn't make use of stuff like namespaces. However I learnt a lot which I'm so happy about. Since I've been able to use that knowledge to create the Luxia engine.
Luxia is also a hobby project that I'm doing on the side, so updates and progress will not be constant and linear. I work when and where I like! 

### Description
Luxia is a Game Engine inspired by Unity and partly by Hazel. It has been created completely by me (as of 2025/11/05). The focus of the Luxia engine is to be usable, stable, well structured, and expandable. A big focus currently is on abstraction, making it so that in the future multiple API's and renderers can be implemented without needing to rewrite the entire engine. Currently, Luxia only supports OpenGL and uses Assimp for model loading and STBI for images (check the vendor folder, a imported libs file might / should be added in the future). 

## How is it built?
### Abstraction
Abstraction is a big thing I'm currently learning about which is being used to ensure expandability. The idea is as follows. OpenGL has its own way of dealing with textures, and vulkan too. So instead of creating two classes for EVERY class that specialises in the different renderers. I instead create an ITexture class which defines LoadFromFile, Use, Delete, etc. And then create a seperate GL_Texture class and VK_Texture class that inherit from ITexture. Now, if in application if i want to create a texture instead of needing to make two application files or say "#ifdef LX_RENDERER_OPENGL create GL_TEXTURE else create VK_TEXTURE". I can instead say "std::shared_ptr<ITexture> tex = Platform::Assets::CreateTexture();" And Platform::Assets:CreateTexture automatically chooses the correct Texture class. Then I can call functions as normal. This goes for IWindow, ICamera, IRenderer, IModel, and more!
```
class ITexture {
  public:
    unsigned int ID;
    virtual void Create() = 0;
}

class GL_Texture : public ITexture {
  public:
    virtual void Create() override { glGenTextures(1, &ID); } 
}
class VK_Texture : public ITexture{
  public:
    virtual void Create() override { vkGenerateTex(1, &ID); }
}

namespace Platform::Assets{
  inline std::shared_ptr<ITexture> CreateTexture(){
    std::shared_ptr<ITexture> tex;
    #ifdef LX_RENDERER_OPENGL
      tex = std::make_shared<GL_Texture>();
    #elif LX_RENDERER_VULKAN
      tex = std::make_shared<VK_Texture>();
    #else
      #error Renderer not supported!
    #endif

    return tex;
  }
}

int main(){
  std::shared_ptr<ITexture> tex = Platform::Assets::CreateTexture();
  tex->Create();
  while(true) { /* loop */ }
}
```

### Layers
The Luxia engine is built using a standard game engine format of using a "**LayerStack**". Basically, the "Run" loop goes through each **layer** in the LayerStack and calls their *OnUpdate*, then re-does the loop but calls *OnRender*. This loop goes from bottom to top. So from Application, to the GameLayer (physics etc), to the RenderLayer.

### Events
The event system is very easiy. Each layer can call PUSH_EVENT(EventType, args...) which pushes the event to the **EventHandler**. At the beginning of each frame the EventHandler sends each event to each layer's *OnEvent* function. Where the layer can decide what to do by creating a dispatcher. 
```
bool mouse_function(MouseButtonPress& e){
  // your fn
  return true; // Is the event consumed
}

void OnEvent(Event& e){
  Dispatcher dispatcher(e);
  
  // *Option 1*
  dispatcher.Dispatch<MouseButtonPress>([&](MouseButtonPress& event) {
				// Your fn
        return true; // Is the event consumed
				});

  // *Option 2*
  dispatcher.Dispatch<MouseButtonPress>(LX_BIND_EVENT_FN(mouse_function));
}
```
**DO NOTE* that the function **must** return wether the event will be consumed or not after the function is called. If it is consumed, then the loop will break and the next layer will NOT recieve the event. Also do note that the events propegate through the layers from **top-to-bottom*, so the opposite of the actual render and update loop!

### IDE
I am using visual studio code 2022 with C++20 and commiting everything to github. The repo is currently private since I've noticed that when I share projects I easily lose motivation since I feel pressure to do what I once wanted to do myself. 

