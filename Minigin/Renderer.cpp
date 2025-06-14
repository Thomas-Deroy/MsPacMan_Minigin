#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <iostream>


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window != nullptr) 
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const RenderParams& params) const
{
	SDL_Texture* sdlTexture = texture.GetSDLTexture();

	SDL_SetTextureColorMod(sdlTexture, params.color.r, params.color.g, params.color.b);
	SDL_SetTextureAlphaMod(sdlTexture, params.color.a);
	SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);

	SDL_Rect dst{};
	dst.x = static_cast<int>(params.x);
	dst.y = static_cast<int>(params.y);

	if (params.width == 0.f || params.height == 0.f)
	{
		SDL_QueryTexture(sdlTexture, nullptr, nullptr, &dst.w, &dst.h);
	}
	else
	{
		dst.w = static_cast<int>(params.width);
		dst.h = static_cast<int>(params.height);
	}

	if (params.useRotation)
	{
		SDL_Point center{ dst.w / 2, dst.h / 2 };
		SDL_RenderCopyEx(GetSDLRenderer(), sdlTexture, nullptr, &dst, params.rotation, &center, params.flip);
	}
	else
	{
		SDL_RenderCopy(GetSDLRenderer(), sdlTexture, nullptr, &dst);
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& dstRect, const SDL_Rect& srcRect, double angle, SDL_RendererFlip flip) const
{
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect, angle, nullptr, flip);
}



SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
