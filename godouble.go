package main

import (
    "bufio"
    "bytes"
    "io/ioutil"
    "os"
)

func main() {

    data, _ := ioutil.ReadFile(os.Args[1])
    // data, _ := ioutil.ReadAll(os.Stdin)
    words := bytes.Split(data, []byte("\n"))
    words = words[:len(words)-1] // remove empty last word due to trailing \n

    w := bufio.NewWriterSize(os.Stdout, 1<<18)

    for _, a := range words {
        for _, b := range words {
            w.Write(a)
            w.Write(b)
            w.WriteRune('\n')
            w.Write(a)
            w.WriteRune(' ')
            w.Write(b)
            w.WriteRune('\n')
        }
    }
    w.Flush()
}
