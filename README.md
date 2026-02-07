# Music Database Management System

A command-line music database management system written in C. Allows users to organize songs, search by various criteria, and create custom playlists.

## Features

**Database Management**
- Add new songs with full metadata (title, artist, album, year, genre, duration)
- Store unlimited songs using dynamic memory allocation
- Persistent storage with file I/O (load and save to text files)

**Search and Filter**
- Search by artist name
- View all songs from a specific album with total duration
- Filter by release year
- Browse by genre (Rock, Pop, Hip-Hop, Jazz, Classical, Electronic, Country, R&B, Metal, Folk, Blues)

**Playlist Features**
- Create custom playlists and save to separate files
- View existing playlists with full song details
- Calculate total playlist duration

**Data Validation**
- Input validation for all fields
- Genre verification against predefined list
- Year range validation (1900-2100)
- Duration format validation (MM:SS)

## Technologies Used

- C Programming Language
- File I/O for data persistence
- Dynamic memory allocation (malloc/realloc)
- String manipulation and validation

## How to Use

Compilation:
gcc music_database.c -o music_db

Running:
./music_db

The program will automatically load existing songs from songs.txt if available, or start with an empty database.

## Menu Options

1. List all songs
2. Add new song
3. Display songs by artist
4. Display songs from album
5. List songs by year
6. List songs by genre
7. Save database
8. Create playlist
9. View playlist
10. Exit

## File Structure

- songs.txt - Main database file (auto-created)
- Custom playlists - User-created playlist files

## Code Features

- Song structure with metadata fields
- Utility functions for input validation and string manipulation
- Database management with dynamic array
- File I/O operations
- Search and filter functions
- Playlist creation and viewing

## Author

Relahi - Computer Science Engineering Student
