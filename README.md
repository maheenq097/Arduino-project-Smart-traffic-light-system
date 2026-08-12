# Arduino Project -- Smart Traffic Light with Pedestrian Crossing

A traffic light system built on an Arduino Uno that behaves like a real intersection: vehicles get a green light by default, and pedestrians can request a safe crossing with a button press. Built on a breadboard, tested both in Tinkercad and on real hardware.

## What it does

- Vehicles get a green light by default. Pedestrians see red.
- Press the button, and the system doesn't just yank the light to red — it goes through a proper transition: green → yellow (warning) → red, the same way a real intersection would.
- Once vehicles have stopped, the pedestrian light turns green and a buzzer beeps once to signal it's safe to cross.
- Near the end of the crossing window, the pedestrian green blinks as a "hurry up" warning, exactly like the countdown walk signals you see on real streets.
- Then it resets back to vehicle green, and waits for the next request.

## Why it's built this way

I didn't want this to just be "LED turns on when button is pressed." The point was to actually mimic the logic real traffic engineers use — nobody wants a system that snaps straight from vehicle-green to pedestrian-green with zero warning. That transition logic is really the whole project; the LEDs are just how you see it working.

## Hardware

| Part | Qty |
|---|---|
| Arduino Uno | 1 |
| Breadboard  | 1 |
| Red LED | 2 |
| Yellow LED | 1 |
| Green LED | 2 |
| 220Ω resistors | 5 |
| Pushbutton | 1 |
| Buzzer | 1 |
| Jumper wires | ~20 |

## Pin mapping

| Signal | Arduino pin |
|---|---|
| Vehicle red LED | 8 |
| Vehicle yellow LED | 9 |
| Vehicle green LED | 10 |
| Pedestrian red LED | 11 |
| Pedestrian green LED | 12 |
| Pushbutton | 2 (interrupt pin) |
| Buzzer | 7 |

## How the code actually works

The core trick here is `attachInterrupt()`. Instead of constantly checking "has the button been pressed?" in a loop (which either wastes cycles or makes the whole light system feel sluggish), the Uno just sits and waits — the button press itself interrupts whatever's happening and sets a flag. The main loop checks that flag and reacts almost instantly.

There's a small debounce window built in too, because mechanical buttons don't switch cleanly — they "bounce" for a few milliseconds and can register as multiple presses if you don't account for it.

## Getting it running

1. Wire up the circuit per the pin table above — LEDs through their resistors, button on pin 2, buzzer on pin 7, everything sharing one common GND rail with the Arduino.
2. Open the sketch in Arduino IDE, select Board → Arduino Uno and the correct COM port.
3. Upload.
4. Press the button and watch the sequence run.

## If something's not working

A few real problems I ran into building this myself, in case you hit the same ones:

- **Nothing lights up at all** — almost always means the code panel is empty or wasn't uploaded, not a wiring issue. Check that first before touching any wires.
- **Button does nothing, ever** — 4-legged pushbuttons have two pairs of legs that are already connected to each other internally. If both your wires land on the same pair, the button physically can't do anything when pressed. Rotate it 90° so your wires sit on two different sides.
- **Button seems to fire randomly with no wires connected** — that's just electrical noise on a floating pin, not a bug. As soon as the button's properly wired, it goes away.

## Ideas for taking this further

This is deliberately left simple so it's easy to extend. Some directions worth trying:

- **LDR (light sensor)** for an automatic night mode
- **Ultrasonic sensor** so the light only stays green if a vehicle is actually waiting
- **LCD or 7-segment display** for a live pedestrian countdown
- **RTC module** for different timing during rush hour vs. late night

## A note on the safety timing

The yellow-light delay before pedestrians get the green light isn't a bug or something to "optimize away" for a faster demo — it's the actual point of the project. Real intersections give drivers a warning before stopping traffic for exactly the same reason. If you want a snappier demo, shortening the delay values in the code is fine; removing the yellow phase entirely defeats what the project is supposed to demonstrate.

---

Built as a learning project — feedback, forks, and "you wired this wrong" comments are all welcome.
