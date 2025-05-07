#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include <iostream>
#include <unordered_map>
#include <SDL.h>

namespace dae
{
    // pImpl pattern
    struct SDLSoundSystem::Impl
    {
        struct SoundRequest
        {
            enum class Type { LOAD, PLAY, VOLUME, STOP, PAUSE, RESUME };
            Type type = Type::LOAD; 
            sound_id id = 0; 
            float volume = 0.0f; 
            std::string path;
            bool loop = false; 
        };

        std::thread m_AudioThread;
        std::mutex m_Mutex;
        std::condition_variable m_Condition;
        std::queue<SoundRequest> m_Queue;
        std::unordered_map<sound_id, Mix_Chunk*> m_SoundCache;
        bool m_IsRunning = true;

        Impl()
        {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
                throw std::runtime_error(std::string("SDL_Init Audio failed: ") + SDL_GetError());

            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                throw std::runtime_error(std::string("Mix_OpenAudio failed: ") + Mix_GetError());

            Mix_AllocateChannels(16);

            // Processing thread
            m_AudioThread = std::thread(&Impl::ProcessQueue, this);
        }

        ~Impl()
        {
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                m_IsRunning = false;
                m_Condition.notify_one();
            }

            if (m_AudioThread.joinable())
                m_AudioThread.join();

            // Clean up loaded sounds
            for (auto& [id, chunk] : m_SoundCache)
                Mix_FreeChunk(chunk);

            Mix_CloseAudio();
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }

        void AddRequest(SoundRequest&& request)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.push(std::move(request));
            m_Condition.notify_one();
        }

        void ProcessQueue()
        {
            while (true)
            {
                SoundRequest request;
                {
                    // Look at Powerpoint for the lock bc this not good
                    std::unique_lock<std::mutex> lock(m_Mutex);
                    m_Condition.wait(lock, [this] { return !m_Queue.empty() || !m_IsRunning; });

                    if (!m_IsRunning && m_Queue.empty())
                        return;

                    if (!m_Queue.empty())
                    {
                        request = std::move(m_Queue.front());
                        m_Queue.pop();
                    }
                }

                ProcessRequest(request);
            }
        }

        void ProcessRequest(const SoundRequest& request)
        {
            switch (request.type)
            {
            case SoundRequest::Type::LOAD:
                LoadSoundImpl(request.id, request.path);
                break;
            case SoundRequest::Type::PLAY:
                PlaySoundImpl(request.id, request.volume, request.loop);
                break;
            case SoundRequest::Type::VOLUME:
                SetVolumeImpl(request.id, request.volume);
                break;
            case SoundRequest::Type::STOP:
                Mix_HaltChannel(-1);
                break;
            case SoundRequest::Type::PAUSE:
                Mix_Pause(-1);
                break;
            case SoundRequest::Type::RESUME:
                Mix_Resume(-1);
                break;
            }
        }

        void LoadSoundImpl(sound_id id, const std::string& path)
        {
            if (m_SoundCache.find(id) != m_SoundCache.end())
                return; // Already loaded

            Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
            if (!sound)
            {
                std::cerr << "Failed to load sound: " << path << " - " << Mix_GetError() << "\n";
                return;
            }
            m_SoundCache[id] = sound;
        }

        void PlaySoundImpl(sound_id id, float volume, bool loop)
        {
            auto it = m_SoundCache.find(id);
            if (it == m_SoundCache.end())
            {
                std::cerr << "Sound ID not found: " << id << "\n";
                return;
            }

            Mix_VolumeChunk(it->second, static_cast<int>(volume * MIX_MAX_VOLUME));
            int loops = loop ? -1 : 0;

            if (Mix_PlayChannel(-1, it->second, loops) == -1)
                std::cerr << "Failed to play sound: " << Mix_GetError() << "\n";
        }

        void SetVolumeImpl(sound_id id, float volume)
        {
            auto it = m_SoundCache.find(id);
            if (it != m_SoundCache.end())
                Mix_VolumeChunk(it->second, static_cast<int>(volume * MIX_MAX_VOLUME));
        }
    };

    // SDLSoundSystem public interface

    SDLSoundSystem::SDLSoundSystem() : m_pImpl(std::make_unique<Impl>()) {}
    SDLSoundSystem::~SDLSoundSystem() = default;

    void SDLSoundSystem::LoadSound(sound_id id, const std::string& path)
    {
        m_pImpl->AddRequest({ Impl::SoundRequest::Type::LOAD, id, 0.0f, path });
    }

    void SDLSoundSystem::Play(sound_id id, float volume, bool loop)
    {
        m_pImpl->AddRequest({ Impl::SoundRequest::Type::PLAY, id, volume, "", loop });
    }

    void SDLSoundSystem::SetVolume(sound_id id, float volume)
    {
        m_pImpl->AddRequest({ Impl::SoundRequest::Type::VOLUME, id, volume, "" });
    }

    void SDLSoundSystem::StopAll()
    {
        m_pImpl->AddRequest({ Impl::SoundRequest::Type::STOP, 0, 0.0f, "" });
    }

    void SDLSoundSystem::PauseAll()
    {
        m_pImpl->AddRequest({ Impl::SoundRequest::Type::PAUSE, 0, 0.0f, "" });
    }

    void SDLSoundSystem::ResumeAll()
    {
        m_pImpl->AddRequest({ Impl::SoundRequest::Type::RESUME, 0, 0.0f, "" });
    }
}