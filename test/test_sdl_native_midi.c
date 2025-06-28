#include <SDL3_native_midi/SDL_native_midi.h>

int main(int argc, char **argv)
{
    int i;

    if (argc == 1) {
        SDL_Log("USAGE: %s [file1.mid] [file2.mid] ...", argv[0]);
        return 1;
    }

    if (!NativeMidi_Init()) {
        SDL_Log("NativeMidi_Init failed: %s", SDL_GetError());
        return 1;
    }

    for (i = 1; i < argc; i++) {
        const char *path = argv[i];
        NativeMidi_Song *song;

        SDL_Log("Loading song '%s' ...", path);
        song = NativeMidi_LoadSong(path);
        if (!song) {
            SDL_Log("Failed to load '%s': %s", path, SDL_GetError());
            continue;
        }

        SDL_Log("Starting song '%s' ...", path);
        NativeMidi_Start(song, 0);

        while (NativeMidi_Active()) {
            SDL_Delay(300);
        }

        SDL_Log("Song '%s' done", path);
        NativeMidi_DestroySong(song);
    }

    SDL_Log("Quitting...");
    NativeMidi_Quit();

    return 0;
}

