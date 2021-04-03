# ParticleSystem
The goal of this project is to create a system of particles of three types (CPU, CPU & GPU, GPU) and compare them in terms of performance.
## Progress
:heavy_check_mark: Particle system on CPU\
:x: Particle system on CPU & GPU\
:x: Particle system on GPU
## Demo
![Demo](https://github.com/Trequend/ParticleSystem/blob/media/Demo.gif)
## Tests
#### Easy (1.5k particles)
![Easy](https://github.com/Trequend/ParticleSystem/blob/media/Easy.png)
#### Normal (50k particles)
![Normal](https://github.com/Trequend/ParticleSystem/blob/media/Normal.png)
#### Hard (350k particles)
![Hard](https://github.com/Trequend/ParticleSystem/blob/media/Hard.png)
## Results
#### Device
- CPU: Intel Core i5 8300H (4x2300 MHz)
- GPU: NVIDIA GeForce GTX 1050 (4 GB)
- RAM: 6 GB
- Resolution: 1920x1080
#### Particle system on CPU (five minutes for each test)

<table>
  <thead>
    <tr>
      <th rowspan="2">Test</th>
      <th colspan="3">FPS</th>
      <th colspan="3">Frame time, ms</th>
      <th colspan="3">Update time (CPU), ms</th>
      <th colspan="3">Render time (CPU), ms</th>
      <th colspan="3">Render time (CPU), ms</th>
    </tr>
    <tr>
      <th>min</th>
      <th>avg</th>
      <th>max</th>
      <th>min</th>
      <th>avg</th>
      <th>max</th>
      <th>min</th>
      <th>avg</th>
      <th>max</th>
      <th>min</th>
      <th>avg</th>
      <th>max</th>
      <th>min</th>
      <th>avg</th>
      <th>max</th>
    </tr>
  </thead>
  <tbody
    <tr>
      <td>Easy</td>
      <td align="center">16</td>
      <td align="center">60</td>
      <td align="center">265</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
    </tr>
    <tr>
      <td>Normal</td>
      <td align="center">35</td>
      <td align="center">60</td>
      <td align="center">72</td>
      <td align="center">12</td>
      <td align="center">15</td>
      <td align="center">27</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">5</td>
      <td align="center">10</td>
      <td align="center">11</td>
      <td align="center">15</td>
      <td align="center">0</td>
      <td align="center">3</td>
      <td align="center">10</td>
    </tr>
    <tr>
      <td>Hard</td>
      <td align="center">7</td>
      <td align="center">8</td>
      <td align="center">8</td>
      <td align="center">126</td>
      <td align="center">132</td>
      <td align="center">148</td>
      <td align="center">49</td>
      <td align="center">52</td>
      <td align="center">66</td>
      <td align="center">76</td>
      <td align="center">80</td>
      <td align="center">93</td>
      <td align="center">&lt 1</td>
      <td align="center">&lt 1</td>
      <td align="center">1</td>
    </tr>
  </tbody>
</table>