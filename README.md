<h1 id="surround-audio-app">Surround audio app</h1>
<p>this program is supposed to simulate 3D audio from a set of headphones.
please make sure to have both headphones in.</p>
<p>-&gt; In order to run these, you will need the .exe file, and the sofa-files folder, so your filesystem would look like this:</p>
<ul>
<li>program folder<ul>
<li>main.exe</li>
<li>guitartune.wav</li>
<li>sofa-files
  <li>MRT01.sofa</li></li>
</ul>
</li>
</ul>
<p>main.exe provides a 3D environment, where the camera spans around a cube - in this case the red cube is the listener, the blue cube is the front of the listener, and the audio comes from the camera&#39;s pov.
To exit, press escape.</p>
<p>test.exe provides a constant anticlockwise rotation of the sound.</p>
<p>If you don&#39;t trust me and want to build these yourself, you&#39;ll need the following dependencies:</p>
<ul>
<li>raylib (main.exe only)</li>
<li>libmysofa</li>
<li>portaudio</li>
<li>libasound2 (linux only)<ul>
<li>libasound2-dev</li>
<li>alsa-tools</li>
<li>alsamixer</li>
</ul>
</li>
</ul>
<h2 id="installation">Installation</h2>
<p>make sure your files look like this:</p>
<ul>
<li>program folder<ul>
<li>main.c</li>
<li>test.c</li>
<li>mysofa.h</li>
<li>portaudio.h</li>
<li>guitartune.wav</li>
<li>sofa-files<ul>
<li>MRT01.sofa</li>
</ul>
</li>
<li>FIRFilter<ul>
<li>firfilter.c</li>
<li>firfilter.h</li>
</ul>
</li>
</ul>
</li>
</ul>
<h3 id="windows-installation">Windows Installation</h3>
<p>For windows, i&#39;d recommend installing msys2, setting it up, and using its gcc compiler
bear in mind this compiler is 64 bit
Once you open msys2:</p>
<pre class="codeblock language-console">pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-libmysofa mingw-w64-x86_64-portaudio
</pre>
<p>then, to build:</p>
<pre class="codeblock language-console">gcc main.c FIRFilter/firfilter.c -o main.exe -lmysofa -lportaudio -lraylib
</pre>
<p>if you&#39;re building test.c, replace main.c with test.c and exclude the -lraylib flag</p>
<h3 id="linux-installation">Linux installation</h3>
<p>I&#39;ll show ubuntu here, but use your own distribution&#39;s package manager
run in console:</p>
<pre class="codeblock language-console">sudo apt-get install libmysofa-dev libmysofa-utils libmysofa1 libasound-dev libasound2-dev portaudio19-dev libportaudio2 alsa-tools alsamixer
</pre>
<p>to install all the dependencies
then to build, simply run</p>
<pre class="codeblock language-console">gcc main.c FIRFilter/firfilter.c -lportaudio -lmysofa -lm -lrt -pthread -lasound
</pre>
<p>alternatively, run:</p>
<pre class="codeblock language-console">sudo apt-get install libasound-dev libasound2-dev alsa-tools alsamixer
</pre>
<p>and omit installing other dependencies: (make sure you have libportaudio.a and libmysofa.a) downloaded</p>
<pre class="codeblock language-console">gcc test.c FIRFilter/firfilter.c libportaudio.a libmysofa.a -lm -lrt -pthread -lasound
</pre>
