# PrayTimes

Implementation of moslem prayer times in C. Based on javascript code from: [PrayTime.js (ver 2.3)](http://praytimes.org/code/).

## Example Usage

```sh
$ praytimes --year=2022 --month=01 --day=24 --timezone=7 --dst=0 --lat=3.58333 --long=97.666667 --elevation=0
Date       Imsak Fajr  Dhuhr Asr   Maghrib Isha
2022-01-24 05:20 05:30 12:41 16:04 18:40   19:48
```

## Building, Installing, & Uninstalling

```sh
$ make all -CFLAG=-O2
$ sudo make install
$ sudo make uninstall
```

## Aliasing

You may create shell alias for more convenient usage.

```sh
alias praytimes="praytimes --lat=3.583333 --long=97.666667 --elevation=0 --timezone=7 --dst=0"
alias today-praytimes="praytimes --year=$(date +\"%Y\") --month=$(date +\"%m\") --day=$(date +\"%d\")"
```

## To Do

- [ ] Cleaner code & file structures (I think this is kinda messed-up)
- [ ] More detailed documentation
- [ ] Better implementation & optimization (?)
- [ ] Better test-case & testing implementations (?)
