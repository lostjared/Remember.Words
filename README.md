# Remember Words

## About the Game

*Remember Words* is a memory game developed from a personal journey of relearning and rehabilitation. After experiencing a coma, the simple yet effective exercise of recalling words was a part of my recovery process with my speech therapist. Inspired by this, I decided to bring the memory aid into a digital format to help others with similar challenges.

## Installation

This game is built using SDL2 and SDL_ttf libraries, so make sure you have these installed on your system. You can download them from [SDL2's website](https://www.libsdl.org/download-2.0.php) and [SDL_ttf's page](https://www.libsdl.org/projects/SDL_ttf/).

### Prerequisites

- SDL2
- SDL_ttf
- A C compiler (GCC or Clang recommended)
- Make

### Building the Game

Clone the repository to your local machine, navigate to the directory containing the game, and compile using a C compiler. For example, using GCC:

```bash
make
```


## Usage

To start the game, run the compiled executable from the terminal:

```bash
./Remember.Words timeout words_list.txt font_size width height
```

- `timeout`: Time in seconds to memorize the words shown.
- `words_list.txt`: Path to a text file containing words used in the game.
- `font_size`: Font size for text rendering.
- `width` and `height`: Dimensions of the game window.

## How to Play

- Press `Space` to start the memory test.
- Memorize the words shown on the screen.
- After the countdown, type the words exactly as shown and press `Enter`.
- The game will indicate whether your recall is correct or not.
- Press `Space` to try again or `Escape` to exit the game.

## Contributing

Contributions to *Remember Words* are welcome! Whether it's bug fixes, feature enhancements, or documentation improvements, feel free to fork the repository and submit a pull request.

## Acknowledgments

Thanks to everyone who has supported this project and contributed to making it a helpful tool for those in need of memory enhancement exercises.
