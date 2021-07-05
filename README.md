# Bee

Bee is a command line utility for Backdrop CMS. It includes commands that allow
developers to interact with Backdrop sites, performing actions like:

- Running cron
- Clearing caches
- Downloading and installing Backdrop
- Downloading, enabling and disabling projects
- Viewing information about a site and/or available projects

Please note that Bee is under active development and, as such, **breaking
changes may occur**. Please see the release notes for a list of any major
changes between versions. Also note that Bee is not (yet?) compatible with
Microsoft Windows.

## Installation

- Download (or clone) Bee to your computer/server.  
  This will create a folder called `bee` with lots of files inside. Your home
  directory is a good location for this folder.

- Make sure `bee/bee.php` is executable.  
  It should be by default, but it doesn't hurt to double-check.

- Make it easy to run `bee` commands.  
  There are two ways to do this:

  1. Put a symlink to `bee/bee.php` in your `$PATH` (e.g.
     `sudo ln -s /path/to/bee/bee.php /usr/local/bin/bee`)

  2. Make a Bash alias for `bee` (e.g. add `alias bee='/path/to/bee/bee.php'` to
     your `.bash_aliases` file)

- Test to make sure it works.  
  Simply type `bee` in your terminal and you should see a list of available
  commands displayed.

### Lando

Bee can also be used with your existing [Lando](https://lando.dev/) setup:

- Add some build steps that download and install Bee:
  ```yaml
  services:
    appserver:
      build:
        - wget -qO bee.zip https://github.com/backdrop-contrib/bee/archive/1.x-1.x.zip
        - unzip -q bee.zip && rm bee.zip
        - mv bee-1.x-1.x /usr/local/bin/bee
  ```

- Add a tooling command for `bee`:
  ```yaml
  tooling:
    bee:
      service: appserver
      cmd: /usr/local/bin/bee/bee.php
  ```

- Rebuild Lando (`lando rebuild`) and then you can use Bee by typing
  `lando bee ...`

## Issues

Bugs and feature requests should be reported in the issue queue:
https://github.com/backdrop-contrib/bee/issues.

## Current Maintainers

- [Peter Anderson](https://github.com/BWPanda)

## Credits

- Originally written for Backdrop CMS by
  [Geoff St. Pierre](https://github.com/serundeputy)  
  (originally called 'Backdrop Console (a.k.a. `b`)').
- Inspired by [Drush](https://github.com/drush-ops/drush).
- [Bee icon](https://thenounproject.com/aomam/collection/bee-emoticons-line/?i=2257433)
  by AomAm from [the Noun Project](http://thenounproject.com).

## License

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
