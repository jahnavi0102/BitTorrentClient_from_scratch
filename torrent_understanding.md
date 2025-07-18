# Torrent file breaking



d8:announce33:http://192.168.1.74:6969/announce7:comment17:Comment goes here10:created by25:Transmission/2.92 (14714)13:creation datei1460444420e8:encoding5:UTF-84:infod6:lengthi59616e4:name9:lorem.txt12:piece lengthi32768e6:pieces40:L@fR���3�K*Ez�>_YS��86��"�&�p�<�6�C{�9G7:privatei0eee



d
  8:announce            33:http://192.168.1.74:6969/announce
  7:comment             17:Comment goes here
  10:created by         25:Transmission/2.92 (14714)
  13:creation date      i1460444420e
  8:encoding            5:UTF-8
  4:info              
    d
      6:length          i59616e
      4:name            9:lorem.txt
      12:piece length   i32768e
      6:pieces          40:L@fR���3�K*Ez�>_YS��86��"�&�p�<�6�C{�9G
      7:private         i0e
    e
e


d8:announce41:http://bttracker.debian.org:6969/announce7:comment35:"Debian CD from cdimage.debian.org"13:creation datei1573903810e9:httpseedsl145:https://cdimage.debian.org/cdimage/release/10.2.0//srv/cdbuilder.debian.org/dst/deb-cd/weekly-builds/amd64/iso-cd/debian-10.2.0-amd64-netinst.iso145:https://cdimage.debian.org/cdimage/archive/10.2.0//srv/cdbuilder.debian.org/dst/deb-cd/weekly-builds/amd64/iso-cd/debian-10.2.0-amd64-netinst.isoe4:infod6:lengthi351272960e4:name31:debian-10.2.0-amd64-netinst.iso12:piece lengthi262144e6:pieces26800:�����PS�^�� (binary blob of the hashes of each piece)ee


d
  8:announce            41:http://bttracker.debian.org:6969/announce
  7:comment             35:"Debian CD from cdimage.debian.org"
  13:creation date      i1573903810e
  4:info
    d
      6:length          i351272960e
      4:name            31:debian-10.2.0-amd64-netinst.iso
      12:piece length   i262144e
      6:pieces          26800:�����PS�^�� (binary blob of the hashes of each piece)
    e
e

# Returned after parsing the buffer
{ announce: "http://bttracker.debian.org:6969/announce" comment: "Debian CD from cdimage.debian.org" created by: "mktorrent 1.1" creation date: 1747482459 info: { length: 709885952 name: "debian-edu-12.11.0-amd64-netinst.iso" piece length: 262144 pieces: <binary string of length 54160> } url-list: [ "https://cdimage.debian.org/cdimage/release/edu/amd64/iso-cd/debian-edu-12.11.0-amd64-netinst.iso" "https://cdimage.debian.org/cdimage/archive/edu/amd64/iso-cd/debian-edu-12.11.0-amd64-netinst.iso" ] }


# Metainfo File Structure

All data in a metainfo file is bencoded. The specification for bencoding is defined above.

The content of a metainfo file (the file ending in ".torrent") is a bencoded dictionary, containing the keys listed below. All character string values are UTF-8 encoded. 0

info: a dictionary that describes the file(s) of the torrent. There are two possible forms: one for the case of a 'single-file' torrent with no directory structure, and one for the case of a 'multi-file' torrent (see below for details)
announce: The announce URL of the tracker (string)
announce-list: (optional) this is an extention to the official specification, offering backwards-compatibility. (list of lists of strings).
The official request for a specification change is here.
creation date: (optional) the creation time of the torrent, in standard UNIX epoch format (integer, seconds since 1-Jan-1970 00:00:00 UTC)
comment: (optional) free-form textual comments of the author (string)
created by: (optional) name and version of the program used to create the .torrent (string)
encoding: (optional) the string encoding format used to generate the pieces part of the info dictionary in the .torrent metafile (string)
Info Dictionary
This section contains the field which are common to both mode, "single file" and "multiple file".

piece length: number of bytes in each piece (integer)
pieces: string consisting of the concatenation of all 20-byte SHA1 hash values, one per piece (byte string, i.e. not urlencoded)
private: (optional) this field is an integer. If it is set to "1", the client MUST publish its presence to get other peers ONLY via the trackers explicitly described in the metainfo file. If this field is set to "0" or is not present, the client may obtain peer from other means, e.g. PEX peer exchange, dht. Here, "private" may be read as "no external peer source".
NOTE: There is much debate surrounding private trackers.
The official request for a specification change is here.
Azureus was the first client to respect private trackers, see their wiki for more details.
Info in Single File Mode
For the case of the single-file mode, the info dictionary contains the following structure:

name: the filename. This is purely advisory. (string)
length: length of the file in bytes (integer)
md5sum: (optional) a 32-character hexadecimal string corresponding to the MD5 sum of the file. This is not used by BitTorrent at all, but it is included by some programs for greater compatibility.
Info in Multiple File Mode
For the case of the multi-file mode, the info dictionary contains the following structure:

name: the name of the directory in which to store all the files. This is purely advisory. (string)
files: a list of dictionaries, one for each file. Each dictionary in this list contains the following keys:
length: length of the file in bytes (integer)
md5sum: (optional) a 32-character hexadecimal string corresponding to the MD5 sum of the file. This is not used by BitTorrent at all, but it is included by some programs for greater compatibility.
path: a list containing one or more string elements that together represent the path and filename. Each element in the list corresponds to either a directory name or (in the case of the final element) the filename. For example, a the file "dir1/dir2/file.ext" would consist of three string elements: "dir1", "dir2", and "file.ext". This is encoded as a bencoded list of strings such as l4:dir14:dir28:file.exte
Notes
The piece length specifies the nominal piece size, and is usually a power of 2. The piece size is typically chosen based on the total amount of file data in the torrent, and is constrained by the fact that too-large piece sizes cause inefficiency, and too-small piece sizes cause large .torrent metadata file. Historically, piece size was chosen to result in a .torrent file no greater than approx. 50 - 75 kB (presumably to ease the load on the server hosting the torrent files).
Current best-practice is to keep the piece size to 512KB or less, for torrents around 8-10GB, even if that results in a larger .torrent file. This results in a more efficient swarm for sharing files. The most common sizes are 256 kB, 512 kB, and 1 MB.
Every piece is of equal length except for the final piece, which is irregular. The number of pieces is thus determined by 'ceil( total length / piece size )'.
For the purposes of piece boundaries in the multi-file case, consider the file data as one long continuous stream, composed of the concatenation of each file in the order listed in the files list. The number of pieces and their boundaries are then determined in the same manner as the case of a single file. Pieces may overlap file boundaries.
Each piece has a corresponding SHA1 hash of the data contained within that piece. These hashes are concatenated to form the pieces value in the above info dictionary. Note that this is not a list but rather a single string. The length of the string must be a multiple of 20.


### Theory: BitTorrent Tracker Communication
The BitTorrent protocol relies on trackers to help peers discover each other. When a client wants to download or seed a torrent, it sends an HTTP GET request (or sometimes POST, though GET is more common for initial announces) to the tracker's announce URL. This request contains various parameters that inform the tracker about the client's state.

Announce Request Parameters (as seen in your code):
info_hash: This is the SHA-1 hash of the "info" dictionary from the torrent file. It uniquely identifies the torrent.

peer_id: A unique 20-byte identifier for your client.

port: The port your client is listening on for incoming peer connections.

uploaded: The total number of bytes uploaded by your client for this torrent.

downloaded: The total number of bytes downloaded by your client for this torrent.

left: The number of bytes remaining to download.

compact: If set to 1, the tracker will return a compact peer list (IP and port concatenated) instead of a dictionary for each peer. This is the common and preferred way.

event (Optional): Specifies the reason for the announce (e.g., started, completed, stopped). If omitted, it's a regular update.

Tracker Response:
The tracker responds with a bencoded dictionary containing information about the torrent and, most importantly, a list of peers.

##### Key elements in the tracker's response:

interval: The minimum number of seconds the client should wait before sending another announce request.

min interval (Optional): A minimum reannounce interval that is smaller than interval and which the client must wait.

peers: This is where the magic happens.

Compact format: If compact=1 was in the request, this will be a string of concatenated 6-byte entries. Each 6-byte entry represents a peer: 4 bytes for the IP address (network byte order) and 2 bytes for the port (network byte order).

Non-compact format: If compact was not used or set to 0, this will be a list of dictionaries, where each dictionary represents a peer and contains peer id, ip, and port. The compact format is almost universally used by modern clients due to its efficiency.

warning message (Optional): A human-readable warning message that clients can display.

failure reason (Optional): If something went wrong, this field will contain a human-readable error message.


### How WriteCallback Operates:

void *contents: This is a pointer to the actual raw data chunk that libcurl just received.

size_t size: The size of a single element (usually 1 byte).

size_t nmemb: The number of elements in the contents buffer. So, the total size of the chunk is size * nmemb.

void *userp: This is the &readBuffer you passed via CURLOPT_WRITEDATA. Inside the callback, you cast it back to a std::string* (i.e., ((std::string*)userp)).

((std::string*)userp)->append((char*)contents, size * nmemb);: This line is the core! It takes the received data chunk (contents with size size * nmemb) and appends it to your readBuffer string.

return size * nmemb;: It's essential that your WriteCallback returns the number of bytes it successfully handled. If it returns a different number (especially 0), libcurl assumes an error occurred and will abort the transfer.


You're dealing with BitTorrent tracker responses, which are primarily Bencoded dictionaries. The key difference between "compact" and "non-compact" responses lies in how the peers information is encoded.

Here's a breakdown of how to parse both:

## Common Tracker Response Structure
Regardless of compact or non-compact, a tracker response is always a Bencoded dictionary. It typically contains these keys:

interval (integer): The minimum number of seconds the client should wait before making another request to the tracker.

complete (integer, optional): The number of seeders (peers with the complete file).

incomplete (integer, optional): The number of leechers (peers still downloading the file).

peers: This is where the difference between compact and non-compact comes in.

failure reason (string, optional): If present, this indicates an error and no other keys should be expected. The value is a human-readable error message.

warning message (string, optional): Similar to failure reason, but the response is still processed normally. A warning message is displayed to the user.

min interval (integer, optional): The absolute minimum announce interval. Clients must not reannounce more frequently than this.

tracker id (string, optional): A string that the client should send back on its next announcements.
