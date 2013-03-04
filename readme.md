# owl

Small Spotify server with a HTTP API to allow remote control of playback (which is local).


## Prerequisite

You need an application key from Spotify in order to connect to their servers. Download
your key and store it in a file at src/spotify_key.h


### OSX

You need libspotify and libevent. On OSX simply use Homebrew to set that up:
    
    brew install libspotify

    brew install libevent


### Linux

Install libspotify from http://www.spotify.com and development headers for libevent using your
distributions package manager.


# Building

To compile and run Owl, run:
    
    make owl
    ./owl

See 'owl -h' for usage.


## Testing

To compile and run the unit test simply run:

    make test

# License

Copyright © 2013 Markus Eliasson

Distributed under the GPLv3 license
