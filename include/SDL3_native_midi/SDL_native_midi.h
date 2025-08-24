/*
  SDL_native_midi: Platform-specific MIDI support.
  Copyright (C) 2000-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/*
 * Several other people have listed copyrights in this code over the years,
 * when it was part of SDL_mixer. The original author was Florian Schulze.
 *
 * Here is a list:
 *
 * Copyright (C) 2000,2001  Florian 'Proff' Schulze <florian.proff.schulze@gmx.net>
 * Copyright (C) 2009 Ryan C. Gordon <icculus@icculus.org>
 * Copyright (C) 2010 Egor Suvorov <egor_suvorov@mail.ru>
 * Copyright (C) 2024 Tasos Sahanidis <code@tasossah.com>
 */

#ifndef SDL_NATIVE_MIDI_H_
#define SDL_NATIVE_MIDI_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_begin_code.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opaque data structure for the song data.
 */
typedef struct NativeMidi_Song NativeMidi_Song;

/**
 * Initialize SDL_native_midi.
 *
 * This should be the first function you call in SDL_native_midi.
 *
 * Unlike other SDL satellite libraries, calls to NativeMidi_Init do not stack,
 * so you really should just call it once and call NativeMidi_Quit once when
 * you are done to deinitialize the library.
 *
 * \returns true if native midi was initialized,
 *          false otherwise.
 *
 * \sa NativeMidi_Quit
 */
extern SDL_DECLSPEC bool SDLCALL NativeMidi_Init(void);

/**
 * Deinitialize SDL_native_midi.
 *
 * This should be the last function you call in SDL_native_midi.
 *
 * Unlike other SDL satellite libraries, calls to NativeMidi_Init do not stack; a
 * single call to NativeMidi_Quit() will deinitialize everything and does not have to
 * be paired with a matching NativeMidi_Init call. For that reason, it's considered
 * best practice to have a single NativeMidi_Init and NativeMidi_Quit call in your
 * program. While this isn't required, be aware of the risks of deviating from that
 * behavior.
 *
 * \sa NativeMidi_Init
 */
extern SDL_DECLSPEC void SDLCALL NativeMidi_Quit(void);

/**
 * Load midi data from an IOStream into a song object.
 *
 * If `closeio` is true, the IOStream will be closed before returning,
 * whether this function succeeds or not. SDL_native_midi reads everything it needs
 * from the IOStream during this call in any case.
 *
 * As a convenience, there is a function to read files from disk without
 * having to deal with SDL_IOStream: `NativeMidi_LoadSong("filename.mid")` will manage
 * those details for you.
 *
 * When done with this song, the application should dispose of it with a call to
 * NativeMidi_DestroySong().
 *
 * \param src an SDL_IOStream that data will be read from.
 * \param closeio true to close/free the SDL_IOStream before returning, false
 *                to leave it open.
 * \returns a pointer to a new song object, or NULL on error.
 *
 * \sa NativeMidi_DestroySong
 */
extern SDL_DECLSPEC NativeMidi_Song * SDLCALL NativeMidi_LoadSong_IO(SDL_IOStream *src, bool closeio);

/**
 * Load midi data from a file into a song object.
 *
 * When done with this song, the application should dispose of it with a call to
 * NativeMidi_DestroySong().
 *
 * \param file a file path from where to load song data.
 * \returns a pointer to a new song object, or NULL on error.
 *
 * \sa NativeMidi_DestroySong
 */
extern SDL_DECLSPEC NativeMidi_Song * SDLCALL NativeMidi_LoadSong(const char *path);

/**
 * Free a song object.
 *
 * If this song is currently playing, it will be stopped.
 *
 * \param song the song object to free.
 *
 * \sa NativeMidi_LoadSong
 * \sa NativeMidi_LoadSong_IO
 */
extern SDL_DECLSPEC void SDLCALL NativeMidi_DestroySong(NativeMidi_Song *song);

/**
 * Play a new song object.
 *
 * There is only ever one song object playing at a time; if this is called
 * when another song object is playing, the currently-playing song is halted
 * and the new song will replace it.
 *
 * \param song the new song object to play.
 * \param loops the number of loops to play the song for.
 *        0 means "play once and stop"
 *        < 0 means "loop forever"
 *
 * \sa NativeMidi_Stop
 */
extern SDL_DECLSPEC void SDLCALL NativeMidi_Start(NativeMidi_Song *song, int loops);

/**
 * Pause the current song.
 * !!! FIXME: this is not hooked up on Haiku OS!
 * (Works on ALSA, macOS, and Windows, though!)
 *
 * Pausing the current song will prevent further playback of the
 * song object, but will maintain the object's current position. When
 * resumed, the song will continue playing from where it left off.
 *
 * A paused song can be resumed by calling NativeMidi_Resume().
 *
 * It is legal to halt a paused song. Playing a new song object when the
 * current song is paused will replace the current song and unpause the
 * music stream.
 *
 * \sa NativeMidi_Resume
 */
extern SDL_DECLSPEC void SDLCALL NativeMidi_Pause(void);

/**
 * Resume playing the current song.
 * !!! FIXME: this is not hooked up on Haiku OS!
 * (Works on ALSA, macOS, and Windows, though!)
 *
 * It is legal to resume an unpaused song; it causes no effect and
 * reports no error.
 *
 * \sa NativeMidi_Pause
 */
extern SDL_DECLSPEC void SDLCALL NativeMidi_Resume(void);

/**
 * Halt playing of the current song.
 *
 * This will stop further playback of the current song. Playback
 * remains stopped until a new song is started, which replaces
 * the current song.
 *
 * \sa NativeMidi_Start
 */
extern SDL_DECLSPEC void SDLCALL NativeMidi_Stop(void);

/**
 * Check the status of the current song.
 *
 * If song is currently playing or paused, this function returns true.
 * Otherwise it returns false.
 *
 * \returns true if a song is active, false otherwise.
 */
extern SDL_DECLSPEC bool SDLCALL NativeMidi_Active(void);

/**
 * Set the volume for the song.
 *
 * The `volume` must be between 0 (silence) and 1 (full volume).
 * If outside that range, volume is clamped to 0 - 1.
 *
 * DICUSSION OF NATIVE MIDI VOLUME PROBLEMS IN WINDOWS:
 *
 * It has been known since Windows Vista that Microsoft changed
 * the behavior of native midi volume control to change the global
 * volume of the entire application. Specifically, if your program
 * has both midi music and sfx with separate volume controls,
 * changing the music volume also changes the sfx volume. Because
 * this problem is at the operating system level, there is likely no
 * SDL-level general solution to this problem that still uses
 * native midi.
 *
 * This volume behavior is clearly undesirable and led many to abandon
 * the OS-level midi playback facilities altogether. In fact, SDL_mixer
 * 3.0.0 spun off native midi to this library and uses other methods to
 * play midi music. You do not need this library to play midi music; it
 * can be done with SDL_mixer directly.
 *
 * Still, although there is no general library-level fix to the volume
 * control problem, there are at least a couple of application-level
 * workarounds if you want to go to the trouble. I'll describe one
 * method in some detail.
 *
 * ONE WORKAROUND:
 *
 * This workaround was inspired by the `sound server` design from the
 * original linux doom source code release. It has the program open
 * a pipe to a child process that plays the music:
 *
 * `game` opens pipe to `game --music`
 *
 * where the parent process `game` is responsible for playing the game
 * and sfx and the child process `game --music` is responsible for playing
 * the music. And it works! Since they are separate processes, they have
 * separate global volumes and changing the volume in one process doesn't
 * affect the volume in the other process.
 *
 * `game --music` is the same executable as `game` including any command-
 * line parameters from `game` but with an additional `--music` parameter
 * to indicate that it should only play music. Alternatively, you could
 * make `game --music` a completely different program (which is what linux
 * doom did). Either way, the parent sends various commands through the
 * pipe to the child, such as:
 *
 * play=7 (where `7` corresponds to which song to play in a list of songs)
 * pause=Y
 * pause=N
 * volume=0.7
 * quit
 *
 * The child reads the commands through stdin and calls the appropriate
 * function to play/alter the music.
 *
 * If your program has a function like:
 *
 *   success = PlaySong(songNum);
 *
 * you would alter it to something like this:
 *
 *   if (parent) {
 *       sprintf(buf, "play=%d\n", songNum);
 *       Use WriteFile to send buf to child
 *       success = use ReadFile to read the response from child
 *   } else {
 *       The child already read the songNum from stdin somewhere else using ReadFile.
 *       success = PlaySong(songNum);
 *       Use WriteFile to send response to stdout
 *   }
 *
 * This particular implementation uses CreateProcess to create the child process
 * with redirected input and output, so opening and using the pipe is not as
 * simple as calling popen, fprintf, pclose. That is an additional complication.
 *
 * Also, before calling NativeMidi_Quit, the child process should call
 * NativeMidi_SetVolume(1.0) so that the application's global volume always
 * ends in a consistent state. This works around another volume problem that
 * started with Windows 11.
 *
 * Other possible workarounds mentioned by others include:
 *
 * 1) Keep track of the volume changes and modify the midi data before it is
 *    sent to the hardware. Of course, now you'll have to parse and modify the
 *    data yourself instead of just sending it to the library. Won't it be
 *    easier to just convert your code to use SDL_mixer 3?
 *
 * 2) Keep track of the music volume changes and modify the sfx volume to
 *    compensate. If the music volume is lowered, raise the sfx volume
 *    proportionally. If the music volume is raised, lower the sfx volume
 *    so that the volume of sfx coming from the speakers is effectively
 *    unchanged. Problems arise if you want to change the music volume to
 *    zero; because the global volume of the application also goes to zero,
 *    sfx volume will be muted no matter how you change the sfx volume.
 *
 * Each workaround has its own complications and problems. Maybe just switch
 * to SDL_mixer 3?
 *
 * \param volume the new volume, between 0 and 1.
 */
extern SDL_DECLSPEC void SDLCALL NativeMidi_SetVolume(float volume);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#include <SDL3/SDL_close_code.h>

#endif /* SDL_NATIVE_MIDI_H_ */
