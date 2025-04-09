import resolve from '@rollup/plugin-node-resolve';
import commonjs from '@rollup/plugin-commonjs';
import typescript from '@rollup/plugin-typescript';
import postcss from 'rollup-plugin-postcss';
import { terser } from 'rollup-plugin-terser';
import peerDepsExternal from 'rollup-plugin-peer-deps-external';
import pkg from './package.json';

export default {
  input: 'index.ts',
  output: [
    {
      file: pkg.main,
      format: 'cjs',
      exports: 'named',
      sourcemap: true,
      sourcemapPathTransform: (relativeSourcePath) => {
        // Fix source map paths to use the correct directory structure
        return relativeSourcePath.replace('../', './').replace(/^\.\//, '@pedrol3001/chess3001-react-components/');
      }
    },
    {
      file: pkg.module,
      format: 'es',
      exports: 'named',
      sourcemap: true,
      sourcemapPathTransform: (relativeSourcePath) => {
        // Fix source map paths to use the correct directory structure
        return relativeSourcePath.replace('../', './').replace(/^\.\//, '@pedrol3001/chess3001-react-components/');
      }
    },
  ],
  plugins: [
    peerDepsExternal(),
    resolve(),
    commonjs(),
    postcss({
      extensions: ['.css'],
      minimize: true,
      inject: {
        insertAt: 'top',
      },
    }),
    typescript({ 
      tsconfig: './tsconfig.json',
      declaration: true,
      declarationDir: 'dist',
      sourceMap: true,
    }),
    terser(),
  ],
  external: ['react', 'react-dom', 'react-chessboard']
};
