# 🎵 *Cognitive Jam: Modeling cognitve bias and information entropy with ML* 🎵

Imagine an model that mimics the human brain, one that starts off, like most of us, preferring simple harmonious tunes and, through experience, learns to love the chaotic brilliance of jazz, represented by dissonant sounds. This repository houses a composite ML model that not only classifies musical structures but also evolves its "taste," much like how humans develop preferences over time.

---

## 🎯 **Mission**: Create a model that promotes learningn to enjoy new sounds despite having preferential bias**

Neurobiologically we understand why humans enjoy consonnat sounds over others, but we want to understand how can we model a system for learning despite this biological preference

Using machine learning, this project dives into how a compsoite network (made with two sub networks) can simulate the evolution of music preferences after "listening" to different harmonic frequencies sequentially in a sound. It combines classification smarts with memory-driven reflection to mimic the journey of learning to appreciate new sounds.

---

## 👨‍🎨 **How the Model Works: Overview of the architecture**


### 🎧 **The Classifier: Mental recognition of the sound**
- **Purpose:** The brain's first checkpoint, understanding "What am I listening to?"
- **Model:** A Binary classification neural network
- **The Underworkings:** This binary classification network takes in the first 10 seconds of a sound and the harmonic frequency at each second and classifies the sound as either consonant or dissonant based on the patterns it recognizes in the data?

### 🧠 **The Listener: Learning to Love**
- **Purpose:** The brain's internal reflection, "how much do I like this sound?
- **Model:** A stacked Long Short-Term Memory network.
- **The Underworkings:** This stacked LSTM takes in the information about the sound and its frequencies sequentially and decides at each time point how does it like this frewuency compared to the overall frequency, previous frequencies it has heard already, and this frequency relative to the last one it just heard.

---
